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

#include "Scheduler.h"

Define_Module(Scheduler);

Scheduler::Scheduler() {
    selfMsg = nullptr;
    lastServedPriority = -1;
}

Scheduler::~Scheduler() {
    cancelAndDelete(selfMsg);
}

void Scheduler::initialize() {
    NrUsers = par("gateSize").intValue();
    selfMsg = new cMessage("selfMsg");
    scheduleAt(simTime(), selfMsg);
    for (int i =0; i < NrUsers; i++)
        userWeights[i] = i + 1;
    for (int i =0; i < NrUsers; i++)
            lastTime[i] = 1;
}

//void Scheduler::handleMessage(cMessage *msg)
//{
//  //  int userWeights[NrUsers];
//    if (msg == selfMsg){
//        for(int i =0;i<NrUsers;i++){
//            cMessage *cmd = new cMessage("cmd");
//            //set parameter value, e.g., nr of blocks to be sent from the queue by user i
//            send(cmd,"txScheduling",i);
//        }
//        scheduleAt(simTime()+par("schedulingPeriod").doubleValue(), selfMsg);
//
//    }
//}
void Scheduler::handleMessage(cMessage *msg) {
    if (msg == selfMsg) {
        int highestPriorityQueue = findNextWeightedNonEmptyQueue();

        if (highestPriorityQueue != -1) {
            cMessage *cmd = new cMessage("cmd");
            lastTime[highestPriorityQueue] = simTime().raw();
            send(cmd, "txScheduling", highestPriorityQueue);
        }

        scheduleAt(simTime() + par("schedulingPeriod").doubleValue(), selfMsg);
    }
}

int Scheduler::findNextWeightedNonEmptyQueue() {
    int64_t maximum = 0;
    int maxiIndex = 0;

    for (int i = 0; i < NrUsers; i++) {
        long long auctionValue = lastTime[i] * userWeight[i];
        if (actionValue > maximum)
            maxiIndex = i;
    }
    cMessage *msg = new cMessage("dummy");
       send(msg, "txScheduling", maxiIndex);

       if (msg->arrivedOn("txPackets")) {
           delete msg;
           return maxiIndex;
       }
    return -1;
}
//int Scheduler::findHighestPriorityNonEmptyQueue() {
//    for (int i = 0; i < NrUsers; i++) {
//        int queueIndex = (lastServedPriority + i + 1) % NrUsers;
//        cMessage *msg = new cMessage("dummy");
//        send(msg, "txScheduling", queueIndex);
//
//        if (msg->arrivedOn("txPackets")) {
//            delete msg;
//            lastServedPriority = queueIndex;
//            return queueIndex;
//        }
//
//        // delete msg;
//    }
//
//    return -1;
//}
