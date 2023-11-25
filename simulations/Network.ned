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

package ipscheduler.simulations;
//import Source;
//import Sink;
//import Scheduler;
//import MyQ;

network Network

{
    parameters:
        int nrHP;
        int nrMP;
        int nrLP;
        @display("bgb=354,294");
    submodules:
        scheduler: Scheduler {
            gateSize = 3;
            @display("p=317,155");
        }
        sink: Sink {
            gateSize = 3;
            @display("p=268,41");
        }
        sources[nrHP + nrMP + nrLP]: Source {
            @display("p=91,220");
        }
        hpq: MyQ {
            NrInputs = parent.nrHP;
            @display("p=91,56");
        }
        mpq: MyQ {
            NrInputs = parent.nrMP;
            @display("p=141,141");
        }
        lpq: MyQ {
            NrInputs = parent.nrLP;
            @display("is=s;p=213,203");
        }


    connections:
        for i=0..nrHP - 1 {
            sources[i].txPackets --> hpq.rxPackets[i];
        }
        for i=0..nrMP-1 {
            sources[nrHP+i].txPackets --> mpq.rxPackets[i];
        }
        for i=0..nrLP-1 {
            sources[nrHP+nrMP+i].txPackets --> lpq.rxPackets[i];
        }
        scheduler.txScheduling[0] --> hpq.rxScheduling;
        scheduler.txScheduling[1] --> mpq.rxScheduling;
        scheduler.txScheduling[2] --> lpq.rxScheduling;
        hpq.txPackets --> sink.rxPackets[0];
        mpq.txPackets --> sink.rxPackets[1];
        lpq.txPackets --> sink.rxPackets[2];
//scheduler.txScheduling[i] --> user[i].rxScheduling;
//user[i].txPackets --> sink.rxPackets[i];

}

