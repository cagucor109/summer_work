#include "ZMQcoms.hpp"
#include <zmq.hpp>
#include <iostream>

int main(){

    ZMQcoms *com = new ZMQcoms();
    (*com).setup(ZMQcoms::PUB, 5555, "connect");
    (*com).publishMessage("hello", "world");

    return 0;
}