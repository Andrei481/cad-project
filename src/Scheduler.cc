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
            lastTime[i] = 0;
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
    for(int i = 0; i<3; i++) {
        if (msg->arrivedOn("rxInfo", i)){
            EV << "ELEMENTS ON INDEX " << i << " " << msg->par("length") << endl;
            elements[i]= msg->par("length");
            delete(msg);
        }
        else if (msg->arrivedOn("rxPriority", i)) {
            lastTime[i] = msg->par("last_time").doubleValue();

            double currPriority = lastTime[i];

            // Emit signal for data collection
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
    double maximum = 3 * (currSimTime - lastTime[2]);
    int maxiIndex = 2;

    for(int i = 2; i>=0 ;i--){
        double currPriority = (i+1) * (double)(currSimTime - lastTime[i]);
        //EV << "BA PULA" << endl; 
        if(elements[i] > 0 && currPriority > maximum){
            maxiIndex = i;
            EV << "Curr max: " << maximum << " updated to " << currPriority << endl;
            maximum = currPriority;
        }

        // if (i == 2) {
        //     EV << "Priority for HP is " << currPriority << endl;
        // }
        // else if (i == 1)
        //     EV << "Priority for MP is " << currPriority << endl;
        // else if (i == 0)
        //     EV << "Priority for LP is " << currPriority << endl;
    }

    return maxiIndex;
}
