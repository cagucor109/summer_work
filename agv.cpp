#include "ZMQcoms.hpp"
#include "Robot.hpp"
#include <iostream>
#include <algorithm>

void publishCompatibility(Robot *agv, ZMQcoms *com, int taskID, int comp);

int main(int argc, char *argv[]){

    ZMQcoms *zmqcom = new ZMQcoms();
    Robot *agv = new Robot();

    int robID;
    std::cout << "Enter the ID for this agv... " << std::endl;
    std::cin >> robID;
    (*agv).setRobotID(robID);

    std::cout << "The following are the positions of the agv..." << std::endl;
    std::cout << (*agv).getLocationX() << " " << (*agv).getCapacity() << " " << (*agv).getBattery() << std::endl;
 
    std::cout << "This instance launches SUB and PUB sockets by default" << std::endl;
    std::cout << "The newTask topic is subscribed to by default" << std::endl;

    int port;
    std::string bind_con;

    std::cout << "\nEnter the port for the SUB socket..." << std::endl;
    std::cin >> port;
    std::cin.ignore();
    std::cout << "\nDo you want to connect or bind?" << std::endl;
    std::cin >> bind_con;

    (*zmqcom).setUp(ZMQcoms::SUB);
    (*zmqcom).setConnection(ZMQcoms::SUB, port, bind_con);
    (*zmqcom).subscribeToTopic("newTask");
    (*zmqcom).subscribeToTopic("bid");

    
    std::cout << "\nEnter the port for the PUB socket..." << std::endl;
    std::cin >> port;
    std::cin.ignore();
    std::cout << "\nDo you want to connect or bind?" << std::endl;
    std::cin >> bind_con;
    (*zmqcom).setUp(ZMQcoms::PUB);
    (*zmqcom).setConnection(ZMQcoms::PUB, port, bind_con);

    std::cout << "Setup success!" << std::endl;

    while(true){
        int taskID, locationStartX, locationEndX, weight, availability;
        int compatibility;
        std::vector<std::string> message;
        std::string topic, data;
        std::stringstream taskss;
        message = (*zmqcom).subscribeMessage();
        topic = message.at(0);
        data = message.at(1);

        if(topic == "newTask"){
            std::cout << "New task received: " << data << std::endl;
            taskss << data;
            taskss >> taskID >> locationStartX >> locationEndX >> weight >> availability;
            (*zmqcom).subscribeToTopic(std::to_string(taskID));        
            (*agv).addSubscription(std::to_string(taskID));
            compatibility = (*agv).findCompatibility(taskID, locationStartX, locationEndX, weight);
            publishCompatibility(agv, zmqcom, taskID, compatibility);
        }
        else if(topic == "bid"){
            std::cout << data << " received on: " << topic << std::endl;
        }
        

    }   

    return 0;
}

void publishCompatibility(Robot *agv, ZMQcoms *com, int taskID, int comp){
    std::string message;
    std::stringstream ss;
    ss << taskID << " " << (*agv).getRobotID() << " " << comp;
    message = ss.str();
    (*com).publishMessage("bid", message);
}