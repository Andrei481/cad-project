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

#include "Sink.h"

Define_Module(Sink);

void Sink::initialize()
{
   // lifetimeSignal = registerSignal("lifetime");
}

void Sink::handleMessage(cMessage *msg)
{
    simtime_t lifetime = simTime() - msg->getCreationTime();
      EV << "Received " << msg->getName() << ", lifetime: " << lifetime << "s" << endl;
    //  emit(lifetimeSignal, lifetime);
      if( msg->arrivedOn("rxPackets", 0)){
          highVector.record(lifetime);
          highHistogram.collect(lifetime);
      }

      if( msg->arrivedOn("rxPackets", 1)){
          mediumVector.record(lifetime);
          mediumHistogram.collect(lifetime);
      }

      if( msg->arrivedOn("rxPackets", 2)){
          lowVector.record(lifetime);
          lowHistogram.collect(lifetime);
      }
      //packetsVector.record(lifetime);
      //histogram.collect(lifetime);
      delete msg;
}

void Sink::finish()
{
        // Print or use the average as needed
        EV << "Average of highVector: " << highHistogram.getMean() << endl;

    // Print or use the average as needed
        EV << "Average of mediumVector: " << mediumHistogram.getMean() << endl;


    // Print or use the average as needed
        EV << "Average of lowVector: " << lowHistogram.getMean() << endl;
}
