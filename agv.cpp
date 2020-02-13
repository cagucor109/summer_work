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
bool receiveBids(Robot *agv, std::string bid);

int main(int argc, char *argv[]){

    Robot *agv = new Robot("Simulation");

    zmq::context_t context (1);

    zmq::socket_t reqSocJoin(context, ZMQ_REQ);
    reqSocJoin.connect("tcp://localhost:5550");

    zmq::socket_t reqSocAssign(context, ZMQ_REQ);
    reqSocAssign.connect("tcp://localhost:5554");

    zmq::socket_t subSocTask(context, ZMQ_SUB);
    subSocTask.connect("tcp://localhost:5551");
    subSocTask.setsockopt(ZMQ_SUBSCRIBE, "newTask", 7);

    zmq::socket_t subSocFwd(context, ZMQ_SUB);
    subSocFwd.connect("tcp://localhost:5553");
    subSocFwd.setsockopt(ZMQ_SUBSCRIBE, "newBid", 6);

    zmq::socket_t pubSoc(context, ZMQ_PUB);
    pubSoc.connect("tcp://localhost:5552");

    zmq::socket_t repSoc(context, ZMQ_REP);
    repSoc.connect("tcp://localhost:5555");

    // join topology

    std::stringstream ss;
    std::string strID;
    int ID;

    ss << "newJoin " << (*agv).getLocationX() << " " << (*agv).getLocationY() << " "
    << (*agv).getCapacity() << " " << (*agv).getBattery() << " " << (*agv).getMaxSpeed() << " " << (*agv).getStatus();

    s_send_nowait(reqSocJoin, ss.str());

    ss.str(""); // clear the string

    strID = s_recv(reqSocJoin); // receive ID
    
    ss << strID;
    ss >> ID;
    (*agv).setRobotID(ID);
    ss.str("");
    ss.clear();

    std::cout << "ID assigned: " << ID << std::endl;

    std::string topic, data;
    std::string assign;
    std::vector<int> bid;
    std::string reqMessage, repMessage;
    std::string taskInfo;

    int slowWork = 0; // slow down motion

    while(true){

        // assignment
        assign = (*agv).checkTimeLimits();
        if(!assign.empty()){
            s_send_nowait(reqSocAssign, assign);
            reqMessage = s_recv(reqSocAssign);
            std::cout << "Received coordinates: " << reqMessage << std::endl;
            (*agv).updateAssignments(reqMessage);
        }

        // new tasks
        topic = s_recv_nowait(subSocTask);
        if(!topic.empty()){
            data = s_recv_nowait(subSocTask);
            if(topic == "newTask"){
                std::cout << "Received new task: " << data << std::endl;
                bid = bidForTask(agv, data);
                // publish bid
                s_sendmore(pubSoc, "newBid");
                s_send_nowait(pubSoc, std::to_string(bid.at(0)) + " " + std::to_string(bid.at(1)) + " " + std::to_string(bid.at(2)));
            }
        }

        // new bids
        topic = s_recv_nowait(subSocFwd);
        if(!topic.empty()){
            data = s_recv_nowait(subSocFwd);
            if(topic == "newBid"){
                std::cout << "Received new bid: " << data <<std::endl;
                receiveBids(agv, data);
            }
        }

        // updates
        repMessage = s_recv_nowait(repSoc);
        if(!repMessage.empty()){
            ss << (*agv).getRobotID() << " " << (*agv).getLocationX() << " " << (*agv).getLocationY() << " " << (*agv).getStatus();
            s_send(repSoc, ss.str());
            ss.str("");
            ss.clear();
        }


        // work
        if(slowWork == 100000){
            slowWork = 0;
            taskInfo = (*agv).workOnAssignments();
            if(!taskInfo.empty()){
                s_send_nowait(reqSocAssign, taskInfo);
                reqMessage = s_recv(reqSocAssign);
                std::cout << reqMessage << std::endl;
            }
        }else{
            slowWork++;
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
    result.push_back((*agv).getRobotID());
    bidValue =(*agv).findCompatibility(locationStartX, locationStartY, locationEndX, locationEndY, weight);
    // create entry with self as best bid to begin with
    (*agv).addSubscription(taskID, (*agv).getRobotID(), bidValue);
    result.push_back(bidValue);
    return result;
}

// update the best bid for a particular task. If agv ID matches that of best bid after a period of time,
// request assignment
bool receiveBids(Robot *agv, std::string bid){
    std::stringstream ss;
    int taskID, robotID, comp;
    ss << bid;
    ss >> taskID >> robotID >> comp;
    (*agv).addSubscription(taskID, robotID, comp);
}