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
    lifetimeHq = registerSignal("lifetimeHq");
    lifetimeMq = registerSignal("lifetimeMq");
    lifetimeLq = registerSignal("lifetimeLq");
    hp_cnt = 0;
}

    void Sink::handleMessage(cMessage *msg)
    {
          simtime_t lifetime = simTime() - msg->getCreationTime();

          EV << "Received " << msg->getName() << ", lifetime: " << lifetime << "s" << endl;

          if (msg->arrivedOn("rxPackets", 2)){
              hp_delay[hp_cnt] = simTime().dbl();
                hp_cnt++;
                EV << "hp_cnt: " << hp_cnt << endl;

                if (hp_cnt == 5) {
                    double hp_avg_delay = 0;
                    for (int i = 0; i < hp_cnt-1; i++) {
                        EV << "hp_cnt[" << i << "]: " << hp_delay[i] << endl;
                        hp_avg_delay += hp_delay[i+1] - hp_delay[i];
                    }

                    hp_avg_delay /= hp_cnt;

                    EV << "hp_avg_delay: " << hp_avg_delay << endl;

                    hp_cnt = 0;

                    cMessage *averageDelayMessage = new cMessage("hp_avg_delay");
                    averageDelayMessage->addPar("hp_avg_delay");
                    averageDelayMessage->par("hp_avg_delay").setDoubleValue(hp_avg_delay);
                    send(averageDelayMessage, "flcDelay");
                }
              emit(lifetimeHq, lifetime);
          }
          else if (msg->arrivedOn("rxPackets", 1)){
              emit(lifetimeMq, lifetime);
          }
          else {
              emit(lifetimeLq, lifetime);
          }

          delete msg;
    }
