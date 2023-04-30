
#ifndef SINK_H_
#define SINK_H_

#include <omnetpp.h>

using namespace omnetpp;

namespace fattree {

/**
 * Implements the Txc simple module. See the NED file for more information.
 */
class Sink : public cSimpleModule
{
  public:
    Sink();
    virtual ~Sink();
  protected:
    uint64_t total_data_size;
    uint64_t total_read_size;
    uint64_t total_write_size;
    simsignal_t ThroughputSignal;
    simsignal_t rThroughputSignal;
    simsignal_t wThroughputSignal;
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();
};

}; // namespace

#endif /* SINK_H_ */
