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

simple Scheduler
{
      parameters:
        @display("i=block/dispatch");
        //volatile 
        //double schedulingPeriod @unit(ms) = default(0.1ms);
        double schedulingPeriod @unit(ms) = default(0.5ms);
        int gateSize=default(3);

    gates:
        output txScheduling[gateSize];
        input rxInfo[gateSize];
        input rxPriority[gateSize];
}
