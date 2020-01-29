#include "ZMQcoms.hpp"
#include "Robot.hpp"
#include <iostream>

std::string checkNewTask(ZMQcoms *com);

int main(int argc, char *argv[]){

    ZMQcoms *zmqcom = new ZMQcoms();
    Robot *agv = new Robot();

    std::cout << "This instance launches SUB and PUB sockets by default" << std::endl;
    std::cout << "The newTask topic is subscribed to by default" << std::endl;

    int port;

    std::cout << "\nEnter the port to connect to SUB socket..." << std::endl;
    std::cin >> port;
    std::cin.ignore();
    (*zmqcom).setup(ZMQcoms::SUB, port);
    (*zmqcom).subscribeToTopic("newTask");

    
    std::cout << "\nEnter the port to bind the PUB socket..." << std::endl;
    std::cin >> port;
    std::cin.ignore();
    (*zmqcom).setup(ZMQcoms::PUB, port);

    std::cout << "Setup success!" << std::endl;

    while(true){
        std::string task;
        task = checkNewTask(zmqcom);
    }   

    return 0;
}

std::string checkNewTask(ZMQcoms *com){
    sleep(2);
    std::vector<std::string> message;
    message = (*com).subscribeMessage();
    if( message.at(0) == "newTask"){
        std::cout << "Received new task : " << message.at(1) << std::endl;
        return message.at(1);
    }else{
        return "";
    }
}