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

namespace fattree;

packet Request {
    char work_type;
    bool finished;
    bool ckp_launched;
    short port_index;
    uint32_t id;
    uint32_t master_id;
    uint32_t num_proc; 
    uint64_t data_size;
    double proc_time;
    string src_addr;
    string des_addr;
    string master_id_addr;
    string next_hop_addr;
    simtime_t generate_time;
    simtime_t arriveModule_time;
    simtime_t leaveModule_time;
    //omnetpp::cPacket pkg;
}