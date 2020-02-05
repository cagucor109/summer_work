/*
This source file manages the connection of agv workers to the overall communication topology.
Functions:  Add a new worker into the network
            Request updates to ensure continued connection to network
*/

#include <iostream>
#include "SQLAPIcoms.hpp"
#include "ZMQcoms.hpp"


int main(int argc, char *argv[]){
    ZMQcoms *zmqcom = new ZMQcoms();
    SQLAPIcoms *sqlcom = new SQLAPIcoms();

    (*zmqcom).setUpPrompt();

    return 0;
}

