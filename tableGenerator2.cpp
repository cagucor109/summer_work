#include "ZMQcoms.hpp"
#include "SQLAPIcoms.hpp"
#include "Robot.hpp"
#include <iostream>

int main(int argc, char *argv[]){

    ZMQcoms *zmqcom = new ZMQcoms();
    SQLAPIcoms *sqlcom = new SQLAPIcoms();

    std::cout << "Enter the desired type of socket..." << std::endl;
    std::cout << "1: PUB\n2: SUB\n3: REQ\n4: REP" << std::endl;

    return 0;
}