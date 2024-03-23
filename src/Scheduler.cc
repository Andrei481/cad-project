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
    //lastServedPriority = -1;
}

Scheduler::~Scheduler() {
    cancelAndDelete(selfMsg);
}

void Scheduler::initialize() {
    NrUsers = par("gateSize").intValue();
    selfMsg = new cMessage("selfMsg");
    scheduleAt(simTime(), selfMsg);
    for (int i =0; i < NrUsers; i++)
        prio[i] = 0;
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
    for(int i = 0; i<NrUsers; i++) {
        if (msg->arrivedOn("rxInfo", i)){
            EV << "ELEMENTS ON QUEUE " << i << " " << msg->par("length") << endl;
            queue[i]= msg->par("length");
            delete(msg);
        }
        else if (msg->arrivedOn("rxPriority", i)) {
            prio[i] = msg->par("queue_prio").doubleValue();

            double currPriority = prio[i];

            delete(msg);
        }
    }
    
    if (msg == selfMsg) {
        int highestPriorityQueue = findNextWeightedNonEmptyQueue();

        cMessage *cmd = new cMessage("cmd");
        send(cmd, "txScheduling", highestPriorityQueue);
        
        scheduleAt(simTime() + par("schedulingPeriod").doubleValue(), selfMsg);
    }
}

int Scheduler::findNextWeightedNonEmptyQueue() {
    double currSimTime = simTime().dbl();
    double maximum = 3 * (currSimTime - prio[2]);
    int maxiIndex = 2;

    for(int i = NrUsers - 1; i>=0 ;i--){
        double currPriority = (i+1) * (double)(currSimTime - prio[i]);
        if(queue[i] > 0 && currPriority > maximum){
            maxiIndex = i;
            EV << "Old max: " << maximum << " New max: " << currPriority << endl;
            maximum = currPriority;
        }

        if (i == 2) {
            EV << "HP prio = " << currPriority << endl;
        }
        else if (i == 1)
            EV << "MP prio = " << currPriority << endl;
        else if (i == 0)
            EV << "LP prio = " << currPriority << endl;
    }
    if(maxiIndex == 2)
        EV << "Sending message to HP Queue with prio = " << maximum << endl;
    else if(maxiIndex == 1)
        EV << "Sending message to MP Queue with prio = " << maximum << endl;
    else
        EV << "Sending message to LP Queue with prio = " << maximum << endl;

    return maxiIndex;
}
