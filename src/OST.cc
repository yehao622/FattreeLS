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
    qLenSignal = registerSignal("queueLen");
    staySignal = registerSignal("stayTime");
}

void OST::handleMessage(cMessage *msg)
{
    Request* req = check_and_cast<Request*>(msg);

    if(!msg->isSelfMessage()){
        int gate_id(intuniform(0, gateSize("port$o")-1, 0));  // randomly select an OSS connected to it
        simtime_t del_time = std::max(gate("port$o", gate_id)->getTransmissionChannel()->getTransmissionFinishTime(), simTime());
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
        if(ost_buffer->isEmpty()){
            if(req->getWork_type() == 'r'){
                req->setLeaveModule_time(del_time + par("read_latency").doubleValue() * (req->getData_size() / (1024.0*1024.0)));
            }else{
                req->setLeaveModule_time(del_time + par("write_latency").doubleValue() * (req->getData_size() / (1024.0*1024.0)));
            }
        }else{
            auto last_req = check_and_cast<Request*>(ost_buffer->back());
            if(req->getWork_type() == 'r'){
                req->setLeaveModule_time(last_req->getLeaveModule_time() + par("read_latency").doubleValue() * (req->getData_size() / (1024.0*1024.0)));
            }else{
                req->setLeaveModule_time(last_req->getLeaveModule_time() + par("write_latency").doubleValue() * (req->getData_size() / (1024.0*1024.0)));
            }
        }
        scheduleAt(req->getLeaveModule_time(), req->dup());
        ost_buffer->insert(req);

    }else{
        ost_buffer->pop(); // send the copy of scheduled msg above, so pop one in queue.
        simtime_t new_del_time = std::max(gate("port$o", req->getPort_index())->getTransmissionChannel()->getTransmissionFinishTime(), simTime());
        emit(staySignal, (new_del_time-req->getArriveModule_time()).dbl());
        sendDelayed(req, new_del_time-simTime(), "port$o", req->getPort_index());
    }

//    if(ost_buffer->getLength())
//        EV << "OST queue: " << ost_buffer->getLength() << " ??????????????????????????\n";

}

uint64_t OST::getDataSizeInQueue(){
    uint64_t total_size(0);
    for (cQueue::Iterator iter(*ost_buffer); !iter.end(); iter++) {
        Request* req = check_and_cast<Request*>(*iter);
        total_size += req->getByteLength(); // Actual data size
    }
    return total_size;
}

void OST::finish(){}

} //namespace
