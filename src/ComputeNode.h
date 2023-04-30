#ifndef COMPUTENODE_H_
#define COMPUTENODE_H_

#include <omnetpp.h>
#include "General.h"

using namespace omnetpp;

namespace fattree {

/**
 * Implements the Txc simple module. See the NED file for more information.
 */
class ComputeNode : public cSimpleModule
{
  public:
    ComputeNode();
    virtual ~ComputeNode();
  protected:
    int gate_to_edge;
    int gate_to_sink;
    uint32_t idx;
    uint32_t receive_cnt;
    uint64_t total_data_size;
    double waitingSignal;
    cQueue* cn_buffer;
    std::unordered_map<uint32_t, std::pair<Request*, uint32_t>> ckp_root_process; // <master_id, <total_number_checkpoint_process, number_finished_checkpoint_processes>>
    std::unordered_set<uint32_t> ckp_ready;
    simsignal_t qLenSignal;
    simsignal_t ThroughputSignal;
    simsignal_t rDurationSignal;
    simsignal_t wDurationSignal;
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();
    void initMsg(Request*, uint32_t);
    void setMsg(Request*);
    bool checkCkpfinished(const uint32_t&);
    std::unordered_set<std::string> selectCkpCNs();
};

}; // namespace

#endif /* COMPUTENODE_H_ */
