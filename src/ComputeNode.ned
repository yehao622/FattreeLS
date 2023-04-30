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

package fattree;

//
// TODO auto-generated module
//
simple ComputeNode
{
    parameters:
        bool sendInitialMessage = default(false);
        double data_size = default(1.0);
        int proc_num = default(2);
        int ckp_cn_num = default(3);
        int ckp_ranks_per_cn = default(4);
        double read_percent = default(1.0);
        double to_cn_chance = default(1.0);
//        double to_ost_chance = default(0.0);
        double ckp_proc_chance = default(0.0);
        volatile double sendInterval @unit(s) = default(exponential(1.0e-2s));
//        volatile double sendReadInterval @unit(s) = default(exponential(1.0e-2s));
//        volatile double sendWriteInterval @unit(s) = default(exponential(1.0e-2s));
        volatile double read_latency @unit(s) = default(exponential(1.66e-4s));
        volatile double write_latency @unit(s) = default(exponential(2.4e-4s));
        int buffer_size_limit @unit(MiB) = default(1024MiB);
        @display("i=device/server2;is=n");
        @signal[queueLen](type="int");
        @signal[readDurationInSystem](type="double");
        @signal[writeDurationInSystem](type="double");
        @signal[waitingTime](type="double");
        @statistic[queueLen](title="Queue Length"; record=histogram);
        @statistic[readDurationInSystem](title="Time duration of read operation in the system"; record=stats,vector);
        @statistic[writeDurationInSystem](title="Time duration of write operation in the system"; record=stats,vector);
        @statistic[waitingTime](title="Queueing time in CN"; record=stats,vector);
    gates:
        inout port[];
}
