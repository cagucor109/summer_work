#include "ZMQcoms.hpp"
#include <iostream>

int main(int argc, char *argv[]){
    ZMQcoms *zmqcom = new ZMQcoms();

    std::cout << "This instance launches SUB and PUB sockets by default" << std::endl;
    std::cout << "The newTask topic is subscribed to by default" << std::endl;

    int port;
    std::string bind_con;

    std::cout << "\nEnter the port for the SUB socket..." << std::endl;
    std::cin >> port;
    std::cin.ignore();
    std::cout << "\nDo you want to connect or bind?" << std::endl;
    std::cin >> bind_con;

    (*zmqcom).setup(ZMQcoms::SUB, port, bind_con);
    (*zmqcom).subscribeToTopic("newTask");

    
    std::cout << "\nEnter the port for the PUB socket..." << std::endl;
    std::cin >> port;
    std::cin.ignore();
    std::cout << "\nDo you want to connect or bind?" << std::endl;
    std::cin >> bind_con;
    (*zmqcom).setup(ZMQcoms::PUB, port, bind_con);

    std::cout << "Setup success!" << std::endl;

    std::cout << "\nEnter signature..." << std::endl;
    std::string signature;
    std::cin >> signature;

    while(true){
        (*zmqcom).publishMessage("newTask", signature);
        sleep(1);
        std::cout << (*zmqcom).subscribeMessage().at(1) << std::endl;
    }

    return 0;
}
