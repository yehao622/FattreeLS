/*
 * OSS.cc
 *
 *  Created on: Feb 3, 2023
 *      Author: hao
 */

#include "OSS.h"

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
    }

    qLenSignal = registerSignal("queueLen");
    staySignal = registerSignal("stayTime");
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
            gate_id = 0;
//            throw cRuntimeError("Another unknown connection to OSS!\n");
        }

        emit(qLenSignal, oss_buffer->getLength());
        req->setPort_index(gate_id);
        req->setArriveModule_time(simTime());
        simtime_t del_time = std::max(gate("port$o", gate_id)->getTransmissionChannel()->getTransmissionFinishTime(), simTime());
        if(oss_buffer->isEmpty()){
            req->setLeaveModule_time(del_time + par("latency").doubleValue());
        }else{
            auto last_req = check_and_cast<Request*>(oss_buffer->back());
            req->setLeaveModule_time(last_req->getLeaveModule_time() + par("latency").doubleValue());
        }
        scheduleAt(req->getLeaveModule_time(), req->dup());
        oss_buffer->insert(req);

    }else{ // if self-msg arrives
        oss_buffer->pop();
        simtime_t new_del_time = std::max(gate("port$o", req->getPort_index())->getTransmissionChannel()->getTransmissionFinishTime(), simTime());
        emit(staySignal, (new_del_time-req->getArriveModule_time()).dbl());
        sendDelayed(req, new_del_time-simTime(), "port$o", req->getPort_index());
    }

//    if(oss_buffer->getLength())
//        EV << "OSS queue: " << oss_buffer->getLength() << " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
}

bool OSS::findOST(std::string ost_name){
    return conn_map.count(ost_name);
}

int OSS::randChoose(std::string layer){
    std::vector<int> all_gates;
    for(auto item:conn_map){
        if (std::regex_match(item.first, std::regex(layer+"\\[[0-9]+\\]"))){
            all_gates.push_back(item.second);
        }
    }

    if(all_gates.empty())
        return -1;
    return all_gates[intuniform(0, all_gates.size()-1, 0)];
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



