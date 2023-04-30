//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "OST.h"
#include "ComputeNode.h"
#include "OSS.h"

namespace fattree {

Define_Module(OST);

OST::OST(){
    ost_buffer = new cQueue(getFullName());
}

OST::~OST(){
    while(!ost_buffer->isEmpty()){
        delete(ost_buffer->pop());
    }
}

void OST::initialize()
{
//    queueIsFull = false;
    qLenSignal = registerSignal("queueLen");
    staySignal = registerSignal("stayTime");
    waitingSignal = registerSignal("waitingTime");
}

void OST::handleMessage(cMessage *msg)
{
    Request* req = check_and_cast<Request*>(msg);

    if(!msg->isSelfMessage()){
        int gate_id(intuniform(0, gateSize("port$o")-1, 0));  // randomly select an OSS connected to it
        req->setArriveModule_time(simTime());
        emit(qLenSignal, ost_buffer->getLength());

        if(req->getWork_type() == 'r'){
            req->setByteLength(req->getData_size());
        }else if(req->getWork_type() == 'w'){
            req->setByteLength(0);
        }else{
            throw cRuntimeError("Need define new rules for type: %c !\n", req->getWork_type());
        }

        req->setPort_index(gate_id);
        double proc_time;
        if(ost_buffer->getLength() < par("proc_num").intValue()){
            if(req->getWork_type() == 'r'){
                proc_time = par("read_latency").doubleValue();
                req->setLeaveModule_time(req->getArriveModule_time() + proc_time * (req->getData_size() / (1024.0*1024.0)));
            }else{
                proc_time = par("write_latency").doubleValue();
                req->setLeaveModule_time(req->getArriveModule_time() + proc_time * (req->getData_size() / (1024.0*1024.0)));
            }
        }else{
            auto last_req = check_and_cast<Request*>(ost_buffer->back());
            if(req->getWork_type() == 'r'){
                proc_time = par("read_latency").doubleValue();
                req->setLeaveModule_time(last_req->getLeaveModule_time() + proc_time * (req->getData_size() / (1024.0*1024.0)));
            }else{
                proc_time = par("write_latency").doubleValue();
                req->setLeaveModule_time(last_req->getLeaveModule_time() + proc_time * (req->getData_size() / (1024.0*1024.0)));
            }
        }
        req->setFinished(true);
        req->setProc_time(proc_time * req->getData_size() / (1024.0*1024.0));
        scheduleAt(req->getLeaveModule_time(), req->dup());
        ost_buffer->insert(req);

    }else{
        delete(ost_buffer->pop()); // send the copy of scheduled msg above, so pop one in queue.
        simtime_t new_del_time = simTime();
        if(gate("port$o", req->getPort_index())->getChannel()->isTransmissionChannel())
            new_del_time = std::max(gate("port$o", req->getPort_index())->getTransmissionChannel()->getTransmissionFinishTime(), new_del_time);

        //        OSS* oss = check_and_cast<OSS*>(gate("port$o", req->getPort_index())->getNextGate()->getOwnerModule());
        //        if(!oss->queueIsFull){
        sendDelayed(req, new_del_time-simTime(), "port$o", req->getPort_index());
        emit(staySignal, (new_del_time - req->getArriveModule_time()).dbl());
        emit(waitingSignal, (new_del_time-req->getArriveModule_time()).dbl() - req->getProc_time());
        //        }else
        //            delete req;
    }

}

uint64_t OST::getDataSizeInQueue(){
    uint64_t total_size(0);
    for (cQueue::Iterator iter(*ost_buffer); !iter.end(); iter++) {
        Request* req = check_and_cast<Request*>(*iter);
        total_size += req->getData_size(); // Actual data size in queue has read from ost(but still in buffer); or to be written to ost(also still in buffer)
    }
    return total_size;
}

void OST::finish(){}

} //namespace
