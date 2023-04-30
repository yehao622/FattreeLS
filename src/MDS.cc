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

#include "MDS.h"
#include "General.h"
#include "OSS.h"
#include "Switch.h"

namespace fattree {

Define_Module(MDS);

MDS::MDS(){}

MDS::~MDS(){
    conn_map.clear();
    comp_map.clear();
    all_OSTs.resize(0);
    all_CNs.resize(0);
}

void MDS::initCollectCompInfo(std::string child, std::string parent){
    Enter_Method("mds get all connection info");
    comp_map[child].insert(parent);
}

void MDS::initialize()
{
    for(int i=0; i<gateSize("port"); i++){
        cGate* g = gate("port$o",i);
        initCollectCompInfo(g->getNextGate()->getOwnerModule()->getFullName(), getName());
        conn_map[g->getNextGate()->getOwnerModule()->getFullName()] = i;

        queue_data_size[i] = 0;
    }
}

void MDS::handleMessage(cMessage *msg)
{
    Request* req = check_and_cast<Request*>(msg);

    std::string rand_oss = randChoose(comp_map[req->getDes_addr()]); // random select an oss that connected to target ost
    req->setNext_hop_addr(rand_oss.c_str());
    std::string rand_core = randChoose(comp_map[rand_oss]); // randomly select a core switch
    send(req, "port$o", conn_map[rand_core]);
}

std::string MDS::randChoose(std::unordered_set<std::string>& name_set){
    int rand_id = intuniform(0, name_set.size()-1, 0);
    auto it = name_set.begin();
    for(int i=0; i<rand_id; i++, it++){}
    return *it;

    // belowing part tries to select the least data size in queue
    int64_t min_data_size(INT64_MAX);
    if( std::regex_match(*name_set.begin(), std::regex("oss\\[[0-9]+\\]")) ){
        std::string dest_oss;
        for(auto oss_name:name_set){
            OSS* oss = check_and_cast<OSS*>(getModuleByPath(oss_name.c_str()));
            auto oss_queue_size = oss->getDataSizeInQueue();
            if(oss_queue_size < min_data_size){
                min_data_size = oss_queue_size;
                dest_oss = oss_name;
            }
        }
        if(dest_oss.empty())
            throw cRuntimeError("MDS cannot decide which oss to choose!\n");
        return dest_oss;
    }

    std::vector<int> core_gates;
    for(auto item:name_set){
        if( std::regex_match(item, std::regex("core\\[[0-9]+\\]")) ){
            core_gates.push_back(conn_map[item]);
            Switch* sw = check_and_cast<Switch*>(gate("port$o", conn_map[item])->getNextGate()->getOwnerModule());
            queue_data_size[conn_map[item]] = sw->getDataSizeInQueue();
        }else{
            throw cRuntimeError("MDS connected with unknown components!\n");
        }
//            EV << getFullName() << " Data size: " << queue_data_size[item.second] << " ====================\n";
    }

    if(core_gates.empty())
        throw cRuntimeError("MDS has no target to deliver!\n");

//    int64_t min_data_size(INT64_MAX);
    int dest_port(-100);

    for(int port_ind:core_gates){
        if(queue_data_size[port_ind] < min_data_size){
            min_data_size = queue_data_size[port_ind];
            dest_port = port_ind;
        }
    }

    return gate("port$o",dest_port)->getNextGate()->getOwnerModule()->getFullName();
}

void MDS::addOSTs(std::string ost_name){
    if(std::find(all_OSTs.begin(), all_OSTs.end(), ost_name) == all_OSTs.end())
        all_OSTs.push_back(ost_name);
}

void MDS::addCNs(std::string cn_name){
    if(std::find(all_CNs.begin(), all_CNs.end(), cn_name) == all_CNs.end())
        all_CNs.push_back(cn_name);
}

std::string MDS::randGetOST(){ // for compute node randomly generating OST target.
    if(all_OSTs.empty())
        throw cRuntimeError("No OST is collected in MDS module!\n");
    return all_OSTs[intuniform(0, all_OSTs.size()-1, 0)];
}

int MDS::getTotalNumCN(){
    return all_CNs.size();
}

std::string MDS::randGetCN(std::string src_cn_name){ // for compute node randomly generating OST target.
    if(all_CNs.empty())
        throw cRuntimeError("No CN is collected in MDS module!\n");

    std::string cn_name = all_CNs[intuniform(0, all_CNs.size()-1, 0)];
    while(cn_name == src_cn_name)
        cn_name = all_CNs[intuniform(0, all_CNs.size()-1, 0)];
    return cn_name;
}

void MDS::finish(){}

}; //namespace
