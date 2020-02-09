/*
    The forwarder allows agvs to communicate to other agvs by forwarding messages
    Receive messages from multiple publishers and forwards to multiple subscribers
*/
#include "ZMQcoms.hpp" // replace with zmq.hpp i.e. decouple
#include <iostream>
#include <algorithm>

int main(int argc, char *argv[]){

    zmq::context_t context (1);

    // PUB socket is the backend, sends messages to agvs
    zmq::socket_t pubSoc (context, ZMQ_PUB);
    pubSoc.bind("tcp://*:5553");

    // SUB socket is the front end, receives messages from agvs
    zmq::socket_t subSoc (context, ZMQ_SUB);
    subSoc.bind("tcp://*:5552");

    // subscribe to all topics (just forwards them)
    subSoc.setsockopt(ZMQ_SUBSCRIBE, "", 0);
    

    std::string topic, data;

    while(true){

        // receive message on the sub socket
        topic = s_recv_nowait (subSoc);
        if(!topic.empty()){
            data = s_recv_nowait (subSoc);

            std::cout << "Received: " << topic << "\t" << data <<std::endl;
            // publish message
            s_sendmore(pubSoc, topic);
            s_send_nowait(pubSoc, data);
        }

    }


    return 0;
}