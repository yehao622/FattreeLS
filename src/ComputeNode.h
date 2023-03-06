/*
 * ComputeNode.h
 *
 *  Created on: Feb 3, 2023
 *      Author: hao
 */

#ifndef COMPUTENODE_H_
#define COMPUTENODE_H_

#include <omnetpp.h>

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
    uint32_t send_cnt;
    uint32_t receive_cnt;
    uint64_t total_data_size;
    uint64_t total_read_size;
    uint64_t total_write_size;
    simsignal_t ThroughputSignal;
    simsignal_t rDurationSignal;
    simsignal_t wDurationSignal;
//    simsignal_t rThroughputSignal;
//    simsignal_t wThroughputSignal;
//    std::vector<double> theory_rdelay_all_components, theory_wdelay_all_components;
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();
};

}; // namespace

#endif /* COMPUTENODE_H_ */
