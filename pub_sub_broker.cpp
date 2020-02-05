#include "ZMQcoms.hpp"
#include <iostream>

int main(int argc, char *argv[]){

    ZMQcoms *zmqcom = new ZMQcoms();

    (*zmqcom).setup(ZMQcoms::SUB, 5555, "bind");
    (*zmqcom).setup(ZMQcoms::PUB, 5556, "bind");
    std::cout << "SUB socket is binded to port 5555" << std::endl;
    std::cout << "PUB socket is binded to port 5556" << std::endl;

    (*zmqcom).subscribeToTopic("newTask");
    (*zmqcom).subscribeToTopic("bid");

    while(true){
        std::vector<std::string> message;
        message = (*zmqcom).subscribeMessage();
        if(!message.at(0).empty()){
            std::cout << "Received and forwarding: " << message.at(1) << " from: " << message.at(0) << std::endl;
            (*zmqcom).publishMessage(message.at(0), message.at(1)); 
        }
        message.at(0).clear();
        message.at(1).clear();
    }

    return 0;
}