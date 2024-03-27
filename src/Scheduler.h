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

// Scheduler.h

// Scheduler.h

#ifndef __SCHED_TST_SCHEDULER_H_
#define __SCHED_TST_SCHEDULER_H_

#include <omnetpp.h>

using namespace omnetpp;

class Scheduler : public cSimpleModule
{
public:
    Scheduler();
    virtual ~Scheduler();

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

private:
    cMessage *selfMsg;
    int NrUsers;
    //int lastServedlastTime;
    int elements[3];
    int lastTime[3];
    int weights[3];
    int findNextWeightedNonEmptyQueue();
};

#endif


