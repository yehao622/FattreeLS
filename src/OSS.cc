/*
 * OSS.cc
 *
 *  Created on: Feb 3, 2023
 *      Author: hao
 */

#include "OSS.h"
#include "OST.h"
#include "Switch.h"

namespace fattree {

Define_Module(OSS);

OSS::OSS(){
    oss_buffer = new cQueue(getFullName());
}

OSS::~OSS(){
    conn_map.clear();
    while(!oss_buffer->isEmpty()){
        delete(oss_buffer->pop());
    }
}

void OSS::initialize()
{
    MDS* mds = check_and_cast<MDS*>(getParentModule()->getSubmodule("mds"));

    for(int i=0; i<gateSize("port"); i++){
        cGate* g = gate("port$o",i);
        std::string lower_module_name = g->getNextGate()->getOwnerModule()->getName();
        conn_map[g->getNextGate()->getOwnerModule()->getFullName()] = i;

        if(strcmp(lower_module_name.c_str(), "ost") == 0){
            mds->initCollectCompInfo(g->getNextGate()->getOwnerModule()->getFullName(), getFullName());
            mds->addOSTs(g->getNextGate()->getOwnerModule()->getFullName());
        }

        queue_data_size[i] = 0;
    }

//    queueIsFull = false;
    qLenSignal = registerSignal("queueLen");
    staySignal = registerSignal("stayTime");
    waitingSignal = registerSignal("waitingTime");
//    free_time = simTime();
}

void OSS::handleMessage(cMessage *msg)
{
    Request* req = check_and_cast<Request*>(msg);

    if(!msg->isSelfMessage()){   // if new msg arrives
        int gate_id;
        if(strcmp(req->getSenderModule()->getName(), "core") == 0){
            gate_id = conn_map[req->getDes_addr()];
        }else if(strcmp(req->getSenderModule()->getName(), "ost") == 0){
            gate_id  = randChoose("core");
        }else{
            throw cRuntimeError("Another unknown connection to OSS!\n");
        }

        emit(qLenSignal, geatRealQueueLength());
        req->setPort_index(gate_id);
        req->setArriveModule_time(simTime());

        bool trans_channel = gate("port$o", gate_id)->getChannel()->isTransmissionChannel();
        simtime_t del_time = simTime();;
        if(trans_channel)
            del_time= std::max(gate("port$o", gate_id)->getTransmissionChannel()->getTransmissionFinishTime(), simTime());

        double proc_time(0.0);
        if(oss_buffer->getLength() < par("proc_num").intValue()){
            if(req->getByteLength()) {
                proc_time = par("latency").doubleValue();
                req->setLeaveModule_time(del_time + proc_time);
            }else{
                req->setLeaveModule_time(del_time);
            }
        }else{
            auto last_req = check_and_cast<Request*>(oss_buffer->back());
            if(req->getByteLength()){
                proc_time = par("latency").doubleValue();
                req->setLeaveModule_time(last_req->getLeaveModule_time() + proc_time);
            }else{
                req->setLeaveModule_time(last_req->getLeaveModule_time());
            }
        }
        req->setProc_time(proc_time);
        scheduleAt(req->getLeaveModule_time(), req->dup());
        oss_buffer->insert(req);

    }else{ // if self-msg arrives
        delete(oss_buffer->pop());
        simtime_t new_del_time = simTime();
        if(gate("port$o", req->getPort_index())->getChannel()->isTransmissionChannel())
            new_del_time = std::max(gate("port$o", req->getPort_index())->getTransmissionChannel()->getTransmissionFinishTime(), simTime());

        std::string next_gate_name = gate("port$o", req->getPort_index())->getNextGate()->getOwnerModule()->getName();
        if(strcmp(next_gate_name.c_str(), "core") == 0){ // if sent to core switch
            //            Switch* sw = check_and_cast<Switch*>(gate("port$o", req->getPort_index())->getNextGate()->getOwnerModule());
            //            if(!sw->queueIsFull){
            sendDelayed(req, new_del_time-simTime(), "port$o", req->getPort_index());
            emit(staySignal, (new_del_time-req->getArriveModule_time()).dbl());
            //            }else
            //                delete req;
        }else if(strcmp(next_gate_name.c_str(), "ost") == 0){
            //            OST* ost = check_and_cast<OST*>(gate("port$o", req->getPort_index())->getNextGate()->getOwnerModule());
            //            if(!ost->queueIsFull){
            sendDelayed(req, new_del_time-simTime(), "port$o", req->getPort_index());
            emit(staySignal, (new_del_time-req->getArriveModule_time()).dbl());
            //            }else
            //                delete req;
        }else{
            throw cRuntimeError("OSS connected with unknown servers!\n");
        }

        emit(waitingSignal, (new_del_time-req->getArriveModule_time()).dbl() - req->getProc_time());
    }

}

bool OSS::findOST(std::string ost_name){
    return conn_map.count(ost_name);
}

int OSS::randChoose(std::string layer){ // select a Core switch
    std::vector<int> all_gates;
    for(auto item:conn_map){
        if (std::regex_match(item.first, std::regex(layer+"\\[[0-9]+\\]"))){
            all_gates.push_back(item.second);
//            Switch* sw = check_and_cast<Switch*>(gate("port$o", item.second)->getNextGate()->getOwnerModule());
//            queue_data_size[item.second] = sw->getDataSizeInQueue();
        }
    }

    if(all_gates.empty())
        throw cRuntimeError("Cannot find available Core. Switch at %s", getName());

    return all_gates[intuniform(0, all_gates.size()-1, 0)];

    // belowing part tries to select the least data size in queue
    int64_t min_data_size(INT64_MAX);
    int dest_port(INT_MIN);

    for(int port_ind:all_gates){
        if(queue_data_size[port_ind] <= min_data_size){
            min_data_size = queue_data_size[port_ind];
            dest_port = port_ind;
        }
    }

    return dest_port;
}

int OSS::geatRealQueueLength(){
    int len(0);
    for (cQueue::Iterator iter(*oss_buffer); !iter.end(); iter++) {
        Request* req = check_and_cast<Request*>(*iter);
        if(req->getByteLength())
            len++;
    }
    return len;
}

uint64_t OSS::getDataSizeInQueue(){
    uint64_t total_size(0);
    for (cQueue::Iterator iter(*oss_buffer); !iter.end(); iter++) {
        Request* req = check_and_cast<Request*>(*iter);
        total_size += req->getByteLength(); // Actual data size
    }
    return total_size;
}

void OSS::finish(){}

}; // namespace



