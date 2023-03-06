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
// Immediately sends out any message it receives. It can optionally generate
// a message at the beginning of the simulation, to bootstrap the process.
//
simple Switch
{
    parameters:
        volatile double core_latency @unit(s) = default(exponential(1.0e-7s));
        volatile double aggr_latency @unit(s) = default(exponential(1.0e-7s));
        volatile double edge_latency @unit(s) = default(exponential(1.0e-7s));
        @display("i=abstract/router;is=n");
        @signal[queueLen](type="int");
        @signal[stayTime](type="double");
        @statistic[queueLen](title="Queue Length"; record=histogram,stats,vector);
        @statistic[stayTime](title="Time staying in Switch"; record=histogram,stats,vector);
    gates:
        inout port[];
}
