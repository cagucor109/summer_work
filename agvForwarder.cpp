/*
    The forwarder allows agvs to communicate to other agvs by forwarding messages
    Receive messages from multiple publishers and forwards to multiple subscribers
*/
#include "ZMQcoms.hpp"
#include <iostream>
#include <algorithm>

int main(int argc, char *argv[]){

    ZMQcoms *zmqcom = new ZMQcoms();

    std::cout << "A forwarder should have at least one SUB and one PUB sockets..." << std::endl;

    (*zmqcom).setUpPrompt();

    std::vector<std::string> message;

    while(true){

        // receive the message
        message = (*zmqcom).subscribeMessage();
        if(message.size() > 0){
            // forward message
            std::cout << "Received: " << message.at(0) << "\t" << message.at(1) << std::endl;
            (*zmqcom).publishMessage(message.at(0), message.at(1));
        }

    }


    return 0;
}