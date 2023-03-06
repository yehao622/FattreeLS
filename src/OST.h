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

#ifndef __FATTREE_OST_H_
#define __FATTREE_OST_H_

#include <omnetpp.h>
#include "MDS.h"

using namespace omnetpp;

namespace fattree {

class OST : public cSimpleModule
{
    public:
    OST();
    virtual ~OST();
    uint64_t getDataSizeInQueue();
  protected:
    cQueue* ost_buffer;
    simsignal_t qLenSignal;
    simsignal_t staySignal;
    simtime_t free_time;
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish();
};

} //namespace

#endif
