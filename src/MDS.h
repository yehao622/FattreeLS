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

#ifndef __FATTREE_MDS_H_
#define __FATTREE_MDS_H_

#include "General.h"

using namespace omnetpp;

namespace fattree {

/**
 * TODO - Generated class
 */
class MDS : public cSimpleModule
{
  public:
    MDS();
    virtual ~MDS();
    void initCollectCompInfo(std::string, std::string);
    void addOSTs(std::string);
    std::string randGetOST();
  protected:
    std::map<int, int64_t> queue_data_size; // <port, data_size_in_queue> for OSS and Cores
    std::unordered_map<std::string, int> conn_map;
    std::unordered_map<std::string, std::unordered_set<std::string>> comp_map;
    std::vector<std::string> all_OSTs;
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish();
    std::string randChoose(std::unordered_set<std::string>&);
};

} //namespace

#endif
