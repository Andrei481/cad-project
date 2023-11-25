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

Scheduler::Scheduler()
{
    selfMsg = nullptr;
}

Scheduler::~Scheduler()
{
    cancelAndDelete(selfMsg);
}

void Scheduler::initialize()
{
    NrUsers = par("gateSize").intValue();
    selfMsg = new cMessage("selfMsg");
    scheduleAt(simTime(), selfMsg);
}

void Scheduler::handleMessage(cMessage *msg)
{
    if (msg == selfMsg) {
        int highestPriorityQueue = findHighestPriorityQueue();

        if (highestPriorityQueue != -1) {
            cMessage *cmd = new cMessage("cmd");
            send(cmd, "txScheduling", highestPriorityQueue);
        }

        scheduleAt(simTime() + par("schedulingPeriod").doubleValue(), selfMsg);
    }
}

int Scheduler::findHighestPriorityQueue()
{
    int highestPriorityQueue = -1;

    for (int i = 0; i < NrUsers; i++) {
        cChannel *channel = gate("txScheduling", i)->getTransmissionChannel();
        cQueue *queue = channel->getQueue();

        if (!channel->isBusy() && queue->getLength() > 0) {
            highestPriorityQueue = i;
            break;
        }
    }

    return highestPriorityQueue;
}

