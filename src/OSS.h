/*
 * OSS.h
 *
 *  Created on: Feb 3, 2023
 *      Author: hao
 */

#ifndef OSS_H_
#define OSS_H_

#include <omnetpp.h>
#include "MDS.h"

using namespace omnetpp;

namespace fattree {

class OSS : public cSimpleModule
{
  public:
    OSS();
    virtual ~OSS();
    bool findOST(std::string);
    uint64_t getDataSizeInQueue();
  protected:
    std::unordered_map<std::string, int> conn_map;
    cQueue* oss_buffer;
    simsignal_t qLenSignal;
    simsignal_t staySignal;
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();
    int randChoose(std::string layer);
};

}; // namespace


#endif /* OSS_H_ */
