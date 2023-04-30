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
simple OSS
{
    parameters:
        int proc_num = default(2);
        int buffer_size_limit @unit(MiB) = default(8192MiB);
        volatile double latency @unit(s) = default(exponential(1.0e-7s));
        @display("i=device/device;is=n");
        @signal[queueLen](type="int");
        @signal[stayTime](type="double");
        @signal[waitingTime](type="double");
        @statistic[queueLen](title="Queue Length"; record=histogram);
        @statistic[stayTime](title="Time staying in OSS"; record=stats,vector);
        @statistic[waitingTime](title="Queueing time in OSS"; record=stats,vector);
    gates:
        inout port[];
}
