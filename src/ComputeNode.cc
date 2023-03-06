/*
 * ComputeNode.cc
 *
 *  Created on: Feb 3, 2023
 *      Author: hao
 */

#include "ComputeNode.h"
#include "MDS.h"

namespace fattree {

Define_Module(ComputeNode);

ComputeNode::ComputeNode(){}

ComputeNode::~ComputeNode(){}

void ComputeNode::initialize()
{
    send_cnt = 0;
    receive_cnt = 0;
    total_data_size = 0;
    total_read_size = 0;
    total_write_size = 0;

   /* theory_rdelay_all_components.resize(4, MM1Delay(3200.0, 10000000.0)); // oss and switches(edge aggr core)
    theory_rdelay_all_components.push_back(MM1Delay(320.0, 699.0)); // OST latency(in OST ned) * 100MB (defined in handmessage() function below)
    theory_rdelay_all_components.push_back(10*1e-7);  // cable delay
    theory_rdelay_all_components.push_back(5*1.0/5120); // data transmission delay(5 routes)

    theory_wdelay_all_components.resize(4, MM1Delay(3200.0, 10000000.0));
    theory_wdelay_all_components.push_back(MM1Delay(320, 417.0));
    theory_wdelay_all_components.push_back(10*1e-7);  // cable delay
    theory_wdelay_all_components.push_back(5*1.0/5120); // data transmission delay(5 routes)*/

    if(par("sendInitialMessage").boolValue() == true){
        Request* req = new Request();
        scheduleAt(simTime(), req);
    }

    ThroughputSignal = registerSignal("throughput");
    rDurationSignal = registerSignal("readDurationInSystem");
    wDurationSignal = registerSignal("writeDurationInSystem");
//    rThroughputSignal = registerSignal("readThroughput");
//    wThroughputSignal = registerSignal("writeThroughput");
}

void ComputeNode::handleMessage(cMessage *msg)
{
    send_cnt++;
    if(send_cnt == UINT32_MAX) {
        EV_ERROR << "Generated workload count has reached the maximum quota: " << UINT32_MAX << "\n";
        endSimulation();
    }

    Request* req = check_and_cast<Request*>(msg);
    MDS* mds = check_and_cast<MDS*>(getParentModule()->getSubmodule("mds"));

    if(req->isSelfMessage()){
        req->setSrc_addr(getFullName());    // set compute node's name
        req->setDes_addr(mds->randGetOST().c_str());       // set target OST
        req->setData_size(1 * 1024 * 1024);     // set data size intended to operate on
        if(uniform(0.0, 1.0, 0) < par("read_percent").doubleValue()){
            req->setWork_type('r');
        }else{
            req->setWork_type('w');
            req->setByteLength(req->getData_size());     // only set data size here for write operations
        }

        simtime_t del_time = gate("port$o", 0)->getTransmissionChannel()->getTransmissionFinishTime();
        if(del_time > simTime()){
            req->setGenerate_time(del_time);
            sendDelayed(req, del_time-simTime(), "port$o", 0);
        }else {
            req->setGenerate_time(simTime());
            send(req, "port$o", 0);
        }

        Request* new_req = new Request();

        simtime_t interval= par("sendInterval").doubleValue();
        scheduleAt(simTime() + interval, new_req);
    }else{
        // do something
        receive_cnt++;
        total_data_size += req->getData_size();

        double thrput = total_data_size / (1024.0 * 1024.0 * simTime().dbl());
        if(req->getWork_type() == 'r') {
            total_read_size += req->getData_size();
            emit(rDurationSignal, (simTime()-req->getGenerate_time()).dbl());
//            emit(rThroughputSignal, total_read_size / (1024.0 * 1024.0 * simTime().dbl()));
        }else{
            total_write_size += req->getData_size();
            emit(wDurationSignal, (simTime()-req->getGenerate_time()).dbl());
//            emit(wThroughputSignal, total_write_size / (1024.0 * 1024.0 * simTime().dbl()));
        }

        emit(ThroughputSignal, thrput);
        delete req;
    }
}

void ComputeNode::finish(){}

}; // namespace


