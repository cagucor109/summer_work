/*
    The AGV should be able to:
        Connect to topology
        Reply to update requests
        Receive new tasks
        Bid for tasks
        Complete tasks
    
    To do this, it needs:   a REQ socket to join topology
                            a REP socket to update database
                            a SUB socket to receive new tasks and bid information
                            a PUB socket to send bid 
*/
#include "ZMQcoms.hpp"
#include "Robot.hpp"
#include <iostream>
#include <algorithm>

std::vector<int> bidForTask(Robot *agv, std::string info);

int main(int argc, char *argv[]){

    ZMQcoms *zmqcom = new ZMQcoms();
    Robot *agv = new Robot("Simulation");

    std::cout << "Robot ID: " << (*agv).getRobotID() << std::endl;
    std::cout << "X location: " << (*agv).getLocationX() << std::endl;
    std::cout << "Y location: " << (*agv).getLocationY() << std::endl;
    std::cout << "Capacity: " << (*agv).getCapacity() << std::endl;
    std::cout << "Battery: " << (*agv).getBattery() << std::endl;

    std::cout << "For an agv, it is recommended to have all sockets active!" << std::endl;

    (*zmqcom).setUpPrompt();

    // join topology

    std::stringstream ss;
    std::string strID;
    int ID;

    ss << "newJoin " << (*agv).getLocationX() << " " << (*agv).getLocationY() << " "
    << (*agv).getCapacity() << " " << (*agv).getBattery();

    (*zmqcom).requestSend(ss.str()); // newJoin message reserved for new agvs
    ss.clear();
    strID = (*zmqcom).requestReceive(); // receive ID
    
    ss << strID;
    ss >> ID;
    (*agv).setRobotID(ID);

    std::vector<std::string> message;
    std::vector<int> bid;

    while(true){
        message = (*zmqcom).subscribeMessage();
        if(message.size() > 0){
            if(message.at(0) == "newTask"){
                bid = bidForTask(agv, message.at(1));
                (*zmqcom).publishMessage(std::to_string(bid.at(0)), std::to_string(bid.at(1)));
            }
            message.clear();
        }
    }


    return 0;
}

std::vector<int> bidForTask(Robot *agv, std::string info){
    std::vector<int> result;
    int bidValue;
    std::stringstream ss;
    int taskID, locationStartX, locationStartY, locationEndX, locationEndY, weight;

    ss << info;
    ss >> taskID >> locationStartX >> locationStartY >> locationEndX >> locationEndY >> weight;
    result.push_back(taskID);
    bidValue =(*agv).findCompatibility(locationStartX, locationStartY, locationEndX, locationEndY, weight);
    result.push_back(bidValue);
    return result;
}