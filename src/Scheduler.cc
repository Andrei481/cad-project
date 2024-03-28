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

}

Scheduler::~Scheduler() {
    cancelAndDelete(selfMsg);
}

void Scheduler::initialize() {
    NrUsers = par("gateSize").intValue();
    selfMsg = new cMessage("selfMsg");
    scheduleAt(simTime(), selfMsg);
    for (int i =0; i < NrUsers; i++){
        lastTime[i] = 0;
        elements[i] = 0;
    }
    weights[0] = 1;
    weights[1] = 5;
    weights[2] = 9;
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
    if(msg->arrivedOn("inputFlc")){
        weights[2] = msg->par("new_weight").longValue();
    }
    
    for(int i = 0; i<NrUsers; i++) {
        if (i == 2){
            int val = weights[2];
            cMessage *weight = new cMessage("weight");
            weight->addPar("weight");
            weight->par("weight").setLongValue(val);
            send(weight, "flcWeight");
        }
        if (msg->arrivedOn("rxInfo", i)){
            //EV << "ELEMENTS ON QUEUE " << i << " " << msg->par("length") << endl;
            elements[i]= msg->par("length");
            delete(msg);
        }
        else if (msg->arrivedOn("rxlastTime", i)) {
            lastTime[i] = msg->par("queue_prio").doubleValue();

            double currlastTime = lastTime[i];

            delete(msg);
        }
    }
    
    if (msg == selfMsg) {
        int highestlastTimeQueue = findNextWeightedNonEmptyQueue();

        cMessage *cmd = new cMessage("cmd");
        send(cmd, "txScheduling", highestlastTimeQueue);
        
        scheduleAt(simTime() + par("schedulingPeriod").doubleValue(), selfMsg);
    }
}

int Scheduler::findNextWeightedNonEmptyQueue() {
    double currSimTime = simTime().dbl();
    double maximum = 0;
    int maxiIndex = 0;

    for(int i = 2; i>=0 ;i--){
        double currlastTime = weights[i] * (double)(currSimTime - lastTime[i]);
        if(elements[i] > 0 && currlastTime > maximum){
            maxiIndex = i;
            EV << "Old max: " << maximum << " New max: " << currlastTime << endl;
            maximum = currlastTime;
        }

        // if (i == 2) {
        //     EV << "HP weight = " << weights[2] << endl;
        // }
        // else if (i == 1)
        //     EV << "MP prio = " << weights[1] << endl;
        // else if (i == 0)
        //     EV << "LP prio = " << weights[0] << endl;
    }
    if(maxiIndex == 2)
        EV << "Sending message to HP Queue with prio = " << maximum << endl;
    else if(maxiIndex == 1)
        EV << "Sending message to MP Queue with prio = " << maximum << endl;
    else
        EV << "Sending message to LP Queue with prio = " << maximum << endl;

    return maxiIndex;
}
