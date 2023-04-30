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

package fattree.simulations;

import fattree.Switch;
import fattree.ComputeNode;
import fattree.OSS;
import fattree.OST;
import fattree.MDS;
import fattree.Sink;
import ned.DatarateChannel;

channel Link extends ned.DatarateChannel
{
    datarate = 40Gbps;
    delay = 0.1us;
    @display("ls=#1A5FB4");
}

channel StorageLink extends ned.DatarateChannel
{
    datarate = 3.22Gbps;
    delay = 0s;
    @display("ls=#E5A50A");
}

//
// Two instances (tic and toc) of Txc connected.
//
network Fattree
{
    parameters:
        @display("bgb=1401.7625,763.795");
        int K_port = default(8); // each switch has 4 ports, make sure 'num_core_oss' strictly less than this variable!
        int pod_oss = default(3); //EACH core switch spares 'num_core_oss' links to OSSes, then (num_core_oss * core.port_num) links availbale to OSSes
        int pod_cn = K_port - pod_oss - 1; // 1 MDS here 

        int num_core = int(K_port * K_port / 4);
        int num_aggr = int(K_port/2 * pod_cn);
        int num_edge = int(K_port/2 * pod_cn);
        int num_cn = int(K_port/2 * K_port/2 * pod_cn);
        int num_oss = default(10); // Can NOT greater than 'pod_oss * K_port/2' 
        int num_ost = default(200);

        int oss_per_ost = default(1); // number of OSS connected by one OST
    submodules:
        core[num_core]: Switch {
            @display("p=305,99,r,20;is=s");
        }

        aggr[num_aggr]: Switch {
            @display("p=207.50499,149.00624,r,20;is=s");
        }

        edge[num_edge]: Switch {
            @display("p=152.31749,204.19374,r,20;is=s");
        }

        cn[num_cn]: ComputeNode {
            @display("p=152.31749,281.45624,m,16,20;is=s");
        }

        oss[num_oss]: OSS {
            @display("p=740.6162,166.66624,r,20;is=s");
        }

        ost[num_ost]: OST {
            @display("p=740.6162,260.485,m,20,20;is=s");
        }
        mds: MDS {
            @display("p=507.72498,18.76375");
        }
        sink: Sink {
            @display("p=147.52501,437.30627;is=s");
        }
    connections allowunconnected:
        for i=0..(num_aggr-1), for j=0..(int(K_port/2-1)) {
            core[(i * int(K_port/2)) % num_core + j].port++ <--> Link <--> aggr[i].port++;
        }

        for i=0..(num_oss-1), for j=0..(int(K_port/2-1)) {
            core[(i * int(K_port/2)) % num_core + j].port++ <--> Link <--> oss[i].port++;
        }

        for i=0..(num_core-1) {
            core[i].port++ <--> mds.port++;
        }

        for i=0..(pod_cn-1), for j=0..(int(K_port/2-1)), for k=0..(int(K_port/2-1)) {
            aggr[i * int(K_port/2) + j].port++ <--> Link <--> edge[i * int(K_port/2) + k].port++;
        }

        for i=0..(num_edge-1), for j=0..(int(K_port/2-1)) {
            edge[i].port++ <--> Link <--> cn[i * int(K_port/2) + j].port++;
        }

        for i=0..(num_ost-1) {
            ost[i].port++ <--> ned.IdealChannel <--> oss[i % num_oss].port++;
        }

		for i=0..(num_cn-1) {
			cn[i].port++ <--> sink.port++;
		}
}
