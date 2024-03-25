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
    hp_cnt = 0, mp_cnt = 0;
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

                EV << "hp_cnt[" << hp_cnt-1 << "]: " << hp_delay[hp_cnt-1] << endl;

                hp_avg_delay /= (hp_cnt - 1);

                EV << "hp_avg_delay: " << hp_avg_delay << endl;

                hp_cnt = 0;

                cMessage *averageDelayMessage = new cMessage("hp_avg_delay");
                averageDelayMessage->addPar("hp_avg_delay");
                averageDelayMessage->par("hp_avg_delay").setDoubleValue(hp_avg_delay);
                send(averageDelayMessage, "flcDelay");
            }


            emit(lifetimeHq, lifetime.dbl());
        }
        else if (msg->arrivedOn("rxPackets", 1)){
            mp_delay[mp_cnt] = simTime().dbl();
            mp_cnt++;
            EV << "mp_cnt: " << mp_cnt << endl;

            if (mp_cnt == 5) {
                double mp_avg_delay = 0;
                for (int i = 0; i < mp_cnt-1; i++) {
                    EV << "delayMq[" << i << "]: " << mp_delay[i] << endl;
                    mp_avg_delay += mp_delay[i+1] - mp_delay[i];
                }

                EV << "mp_delay[" << mp_cnt-1 << "]: " << mp_delay[mp_cnt-1] << endl;

                mp_avg_delay /= (mp_cnt - 1);

                EV << "mp_avg_delay: " << mp_avg_delay << endl;

                mp_cnt = 0;

                cMessage *averageDelayMessage = new cMessage("mp_avg_delay");
                averageDelayMessage->addPar("mp_avg_delay");
                averageDelayMessage->par("mp_avg_delay").setDoubleValue(mp_avg_delay);
                send(averageDelayMessage, "flcDelay");
            }

            emit(lifetimeMq, lifetime.dbl());
        }
        else {
            emit(lifetimeLq, lifetime.dbl());
        }

        delete msg;
    }
