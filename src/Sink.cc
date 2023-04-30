#include "Sink.h"
#include "MDS.h"

namespace fattree {

Define_Module(Sink);

Sink::Sink(){}

Sink::~Sink(){}

void Sink::initialize()
{
    total_data_size = 0;
    total_read_size = 0;
    total_write_size = 0;

    ThroughputSignal = registerSignal("throughput");
    rThroughputSignal = registerSignal("readThroughput");
    wThroughputSignal = registerSignal("writeThroughput");
}

void Sink::handleMessage(cMessage *msg)
{
    Request* req = check_and_cast<Request*>(msg);

    total_data_size += req->getData_size();
    if(req->getWork_type() == 'r') {
        total_read_size += req->getData_size();
        emit(rThroughputSignal, total_read_size / (1024.0 * 1024.0 * simTime().dbl()));
    }else{
        total_write_size += req->getData_size();
        emit(wThroughputSignal, total_write_size / (1024.0 * 1024.0 * simTime().dbl()));
    }

    double thrput = total_data_size / (1024.0 * 1024.0 * simTime().dbl());
    emit(ThroughputSignal, thrput);
    delete req;
}

void Sink::finish(){}

}; // namespace




