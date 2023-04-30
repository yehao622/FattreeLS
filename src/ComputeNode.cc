#include "ComputeNode.h"
#include "MDS.h"
//#include "Switch.h"

namespace fattree {

Define_Module(ComputeNode);

ComputeNode::ComputeNode(){
    cn_buffer = new cQueue(getFullName());
}

ComputeNode::~ComputeNode(){
    while(!cn_buffer->isEmpty()){
        delete(cn_buffer->pop());
    }
}

void ComputeNode::initialize()
{
    idx = 1;

    if(par("sendInitialMessage").boolValue() == true){
        Request* req = new Request();
        req->setFinished(false);
        scheduleAt(simTime(), req);
    }

    for(int i=0; i<2; i++){
        cGate* g = gate("port$o",i);
        std::string lower_module_name = g->getNextGate()->getOwnerModule()->getName();
        if(strcmp(lower_module_name.c_str(), "edge") == 0)
            gate_to_edge = i;
        else
            gate_to_sink = i;
    }

    MDS* mds = check_and_cast<MDS*>(getParentModule()->getSubmodule("mds"));
    mds->addCNs(getFullName());

    qLenSignal = registerSignal("queueLen");
    rDurationSignal = registerSignal("readDurationInSystem");
    wDurationSignal = registerSignal("writeDurationInSystem");
    waitingSignal = registerSignal("waitingTime");
}

void ComputeNode::handleMessage(cMessage *msg)
{
    if(idx == UINT32_MAX) {
        EV_ERROR << "Generated workload count has reached the maximum quota: " << UINT32_MAX << "\n";
        endSimulation();
    }

    Request* req = check_and_cast<Request*>(msg);
//    Switch* sw = check_and_cast<Switch*>(gate("port$o", 0)->getNextGate()->getOwnerModule());

    if(req->isSelfMessage()){
        if(!req->getFinished()){
            initMsg(req, idx);
            idx++;
            Request* new_req = new Request();
            new_req->setFinished(false);
            simtime_t interval = par("sendInterval").doubleValue();
            scheduleAt(simTime() + interval, new_req);
        }else{
            delete(cn_buffer->pop());
            simtime_t del_time = simTime();
            if(gate("port$o", gate_to_edge)->getChannel()->isTransmissionChannel())
                del_time = std::max(gate("port$o", gate_to_edge)->getTransmissionChannel()->getTransmissionFinishTime(), simTime());

            emit(waitingSignal, (del_time-req->getArriveModule_time()).dbl() - req->getProc_time());
            sendDelayed(req, del_time-simTime(), "port$o", gate_to_edge);
        }

    }else{ // return from edge switches
        req->setArriveModule_time(simTime());
        MDS* mds = check_and_cast<MDS*>(getParentModule()->getSubmodule("mds"));

        if(strcmp(req->getSrc_addr(), getFullName()) == 0) {
            if(std::regex_match(req->getMaster_id_addr(), std::regex("cn\\[[0-9]+\\]"))){ // checkpoint process
                if(!req->getCkp_launched()){ // root checkpoint process begin to work now
                    if(req->getWork_type() == 'w')
                        req->setByteLength(req->getData_size());
                    req->setDes_addr(mds->randGetOST().c_str());//(mds->randGetCN(getFullName()).c_str());
                    req->setCkp_launched(true);
                    ckp_root_process[req->getMaster_id()].first = req->dup();
                    ckp_root_process[req->getMaster_id()].first->setData_size(0);

                    simtime_t del_time = simTime();
                    if(gate("port$o", gate_to_edge)->getChannel()->isTransmissionChannel())
                        del_time = std::max(gate("port$o", gate_to_edge)->getTransmissionChannel()->getTransmissionFinishTime(), simTime());
                    sendDelayed(req, del_time-simTime(), "port$o", gate_to_edge);
                }else{
                    if(strcmp(req->getMaster_id_addr(), getFullName()) == 0){ // a checkpoint process comes back to the root
                        ckp_root_process[req->getMaster_id()].second++;
                        int64_t accu_byte_size, accu_data_size;
                        if(req->getMaster_id() == req->getId()){
                            ckp_ready.insert(req->getMaster_id());
                        }
                        accu_byte_size = ckp_root_process[req->getMaster_id()].first->getByteLength();
                        accu_data_size = ckp_root_process[req->getMaster_id()].first->getData_size();
                        int ckp_proc_num = std::max(ckp_root_process[req->getMaster_id()].first->getNum_proc(), req->getNum_proc());
                        ckp_root_process[req->getMaster_id()].first = req->dup();
                        ckp_root_process[req->getMaster_id()].first->setNum_proc(ckp_proc_num);
                        ckp_root_process[req->getMaster_id()].first->setByteLength(accu_byte_size + req->getByteLength());
                        ckp_root_process[req->getMaster_id()].first->setData_size(accu_data_size + req->getData_size());

                        if(checkCkpfinished(req->getMaster_id())){
                            send(ckp_root_process[req->getMaster_id()].first, "port$o", gate_to_sink);
                            ckp_ready.erase(req->getMaster_id());
                            ckp_root_process.erase(req->getMaster_id());
                        }
                        delete(req);
                    }else{ // non-root process finish, and need to go to Master_id_addr()
                        req->setSrc_addr(req->getMaster_id_addr());
                        simtime_t del_time = simTime();
                        if(gate("port$o", gate_to_edge)->getChannel()->isTransmissionChannel())
                            del_time = std::max(gate("port$o", gate_to_edge)->getTransmissionChannel()->getTransmissionFinishTime(), simTime());
                        sendDelayed(req, del_time-simTime(), "port$o", gate_to_edge);
                    }
                }
            }else{ // non-checkpoint process finished
                if(req->getWork_type() == 'r') {
                    emit(rDurationSignal, (simTime()-req->getGenerate_time()).dbl());
                }else{
                    emit(wDurationSignal, (simTime()-req->getGenerate_time()).dbl());
                }
                send(req, "port$o", gate_to_sink);
            }
        }else if(strcmp(req->getDes_addr(), getFullName()) == 0){
            if(std::regex_match(req->getMaster_id_addr(), std::regex("cn\\[[0-9]+\\]")) &&
                    !req->getCkp_launched()) { // checkpoint process from root

                    if(req->getWork_type() == 'w')
                        req->setByteLength(req->getData_size());
                    req->setSrc_addr(getFullName());
                    req->setDes_addr(mds->randGetOST().c_str());//(mds->randGetCN(getFullName()).c_str());
                    req->setCkp_launched(true);

                    simtime_t del_time = simTime();
                    if(gate("port$o", gate_to_edge)->getChannel()->isTransmissionChannel())
                        del_time = std::max(gate("port$o", gate_to_edge)->getTransmissionChannel()->getTransmissionFinishTime(), simTime());
                    sendDelayed(req, del_time-simTime(), "port$o", gate_to_edge);
            }else{
                double proc_time(0.0);
                simtime_t del_time = simTime();//std::max(gate("port$o", gate_to_edge)->getTransmissionChannel()->getTransmissionFinishTime(), simTime());
                if(req->getWork_type() == 'r') {
                    req->setByteLength(req->getData_size());
                    proc_time = par("read_latency").doubleValue();
                    if(cn_buffer->getLength() < par("proc_num").intValue()){
                        req->setLeaveModule_time(del_time + proc_time * (req->getData_size() / (1024.0*1024.0)));
                    }else{
                        auto last_req = check_and_cast<Request*>(cn_buffer->back());
                        req->setLeaveModule_time(last_req->getLeaveModule_time() + proc_time * (req->getData_size() / (1024.0*1024.0)));
                    }
                }else{
                    req->setByteLength(0);
                    proc_time = par("write_latency").doubleValue();
                    if(cn_buffer->getLength() < par("proc_num").intValue()){
                        req->setLeaveModule_time(del_time + proc_time * (req->getData_size() / (1024.0*1024.0)));
                    }else{
                        auto last_req = check_and_cast<Request*>(cn_buffer->back());
                        req->setLeaveModule_time(last_req->getLeaveModule_time() + proc_time * (req->getData_size() / (1024.0*1024.0)));
                    }
                }
                req->setProc_time(proc_time * req->getData_size() / (1024.0*1024.0));
                req->setFinished(true);
                scheduleAt(req->getLeaveModule_time(), req->dup());
                cn_buffer->insert(req);
            }
        }else{
            throw cRuntimeError("There is a request that is not supposed here!\n");
        }

        emit(qLenSignal, cn_buffer->getLength());
    }
}

void ComputeNode::setMsg(Request* req){
    req->setData_size((uint64_t)(par("data_size").doubleValue() * 1024 * 1024));     // set data size intended to operate on

    double read_prob = uniform(0.0, 1.0, 0);
    if(read_prob < par("read_percent").doubleValue()){
        req->setWork_type('r');
    }else{
        req->setWork_type('w');
        if(!std::regex_match(req->getMaster_id_addr(), std::regex("cn\\[[0-9]+\\]"))) // Initially checkpoint process not set package size
            req->setByteLength(req->getData_size());     // only set data size here for write operations
    }

    simtime_t del_time = simTime();
    if(gate("port$o", gate_to_edge)->getChannel()->isTransmissionChannel())
        del_time = std::max(gate("port$o", gate_to_edge)->getTransmissionChannel()->getTransmissionFinishTime(), simTime());
    req->setGenerate_time(simTime());
    sendDelayed(req, del_time-simTime(), "port$o", gate_to_edge);
}

void ComputeNode::initMsg(Request* req, uint32_t id){
    MDS* mds = check_and_cast<MDS*>(getParentModule()->getSubmodule("mds"));
    req->setSrc_addr(getFullName());    // set compute node's name
    req->setId(id);
    req->setMaster_id(id);
    req->setNum_proc(0);
    req->setCkp_launched(false);

    double prob = uniform(0.0, 1.0, 0);
    if(prob < par("to_cn_chance").doubleValue()){
        req->setDes_addr(mds->randGetCN(getFullName()).c_str()); // set target to another CN
        setMsg(req);
    }else if(prob < par("to_cn_chance").doubleValue() + par("ckp_proc_chance").doubleValue()){ // checkpoint processes
        auto cn_selected = selectCkpCNs();
        int total_proc = cn_selected.size() * par("ckp_ranks_per_cn").intValue();
        for(auto cn_name:cn_selected) {
            for(int i=0; i<par("ckp_ranks_per_cn").intValue(); i++){
                Request* new_req = req->dup();
                new_req->setMaster_id_addr(getFullName());
                new_req->setDes_addr(cn_name.c_str());
                if(!i && std::strcmp(cn_name.c_str(), getFullName())==0){ // root checkpoint process
                    new_req->setNum_proc(total_proc);
                    ckp_root_process[new_req->getMaster_id()].first = new_req;//new_req->dup();
                    ckp_root_process[new_req->getMaster_id()].second = 0;
                }else{
                    new_req->setId(UINT32_MAX);
                }
                setMsg(new_req);
            }
        }
        delete req;
    }else{
        req->setDes_addr(mds->randGetOST().c_str());       // set target OST
        setMsg(req);
    }
}

bool ComputeNode::checkCkpfinished(const uint32_t& root_id){
    if(!ckp_root_process.count(root_id) || !ckp_ready.count(root_id))
        return false;

    return ckp_root_process[root_id].first->getNum_proc() == ckp_root_process[root_id].second;
}

std::unordered_set<std::string> ComputeNode::selectCkpCNs(){
    std::unordered_set<std::string> res;
    int cn_num = par("ckp_cn_num").intValue();
    MDS* mds = check_and_cast<MDS*>(getParentModule()->getSubmodule("mds"));
    assert(cn_num < mds->getTotalNumCN());
    while(res.size() < cn_num){
        std::string cn_chosen = mds->randGetCN(getFullName());
        if(!res.count(cn_chosen))
            res.insert(cn_chosen);
    }

    res.insert(getFullName());
    return res;
}

void ComputeNode::finish(){
    ckp_ready.clear();
    ckp_root_process.clear();
}

}; // namespace


