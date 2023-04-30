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
simple OST
{
    parameters:
		int proc_num = default(2);
		int buffer_size_limit @unit(MiB) = default(1024MiB);
        volatile double read_latency @unit(s) = default(exponential(1.43e-3s)); // read BW 1 / 1.43e-3 MB/s
        volatile double write_latency @unit(s) = default(exponential(2.4e-3s)); // write BW 1 / 2.4e-3 MB/s 
        @display("i=device/disk");
        @signal[queueLen](type="int");
        @signal[stayTime](type="double");
        @signal[waitingTime](type="double");
        @statistic[queueLen](title="Queue Length"; record=histogram);
        @statistic[stayTime](title="Time staying in OST"; record=stats,vector);
        @statistic[waitingTime](title="Queueing time in OST"; record=stats,vector);
    gates:
        inout port[];
}
