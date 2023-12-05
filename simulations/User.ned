// User.ned
//import MyQ;
//import Source;
package ipscheduler.simulations;

module User
{
    parameters:
        int priority;  // Priority level (1, 2, 3, or 4)
        string behavior;  // Behavior type ("random", "file", etc.)
        int userId;  // Unique user ID

    gates:
        output txPackets;
        input rxScheduling;

    submodules:
        generator: Source;
        myqq: MyQ;

    connections:
        rxScheduling --> myqq.rxScheduling;
        //generator.txPackets --> myqq.rxPackets;
        myqq.txPackets --> txPackets;
}
