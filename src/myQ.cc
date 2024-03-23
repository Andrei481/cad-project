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

#include "myQ.h"

Define_Module(MyQ);

void MyQ::initialize()
{
    queue.setName("queue");
}

void MyQ::handleMessage(cMessage *msg)
{
    //int i;
    int queue_len;
    //ql = queue.getLength();
    if (msg->arrivedOn("rxPackets")){
        queue.insert(msg);
    } else if (msg->arrivedOn("rxScheduling")){
        delete msg;

        if (!queue.isEmpty()){
          msg = (cMessage *)queue.pop();
          send(msg, "txPackets");
        }

        cMessage *queuePrio = new cMessage("queuePrio");
        queuePrio->addPar("queue_prio");
        queuePrio->par("queue_prio").setDoubleValue(simTime().dbl());
        send(queuePrio, "txPriority");
    }

    queue_len = queue.getLength();

    cMessage *qInfo = new cMessage("qInfo");
    qInfo->addPar("length");
    qInfo->par("length").setLongValue(queue_len);
    sendDelayed(qInfo, 0.1, "txInfo");
}
