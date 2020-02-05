#include "ZMQcoms.hpp"
#include <zmq.hpp>
#include <iostream>

int main(){

    ZMQcoms *com = new ZMQcoms();
    (*com).setup(ZMQcoms::SUB, 5555, "bind");
    (*com).subscribeToTopic("hello");
    std::cout << (*com).subscribeMessage().at(1);
    
    return 0;
}