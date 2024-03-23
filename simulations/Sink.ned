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

//
// TODO auto-generated module
//

package ipscheduler.simulations;

simple Sink
{
 parameters:
        int gateSize=default(3);
        @display("i=block/sink");
      //  @signal[lifetime](type="simtime_t");
      //  @statistic[lifetime](title="lifetime of arrived jobs"; unit=s; record=vector,mean,max; interpolationmode=none);
        @signal[lifetimeHq](type="double");
        @signal[lifetimeMq](type="double");
        @signal[lifetimeLq](type="double");
        @statistic[statsHq](title="Lifetime of HP messages"; source="lifetimeHq"; record=vector,stats; interpolationmode=none);
        @statistic[statsMq](title="Lifetime of MP messages"; source="lifetimeMq"; record=vector,stats; interpolationmode=none);
        @statistic[statsLq](title="Lifetime of LP messages"; source="lifetimeLq"; record=vector,stats; interpolationmode=none);
    gates:
        input rxPackets[gateSize];

}
