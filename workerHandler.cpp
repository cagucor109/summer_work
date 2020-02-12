/*
This source file manages the connection of agv workers to the overall communication topology.
Functions:  Add a new worker into the network
            Request updates to ensure continued connection to network
Requirements:   Should have at least a request and reply socket to communicate with workers
*/

#include <iostream>
#include <typeinfo>
#include "SQLAPIcoms.hpp"
#include "ZMQcoms.hpp"

void processRequestMessage(SQLAPIcoms *sqlcom, std::string reqMessage);
std::string processReplyMessage(SQLAPIcoms *sqlcom, std::string repMessage);

int main(int argc, char *argv[]){
    SQLAPIcoms *sqlcom = new SQLAPIcoms();

    zmq::context_t context (1);
    
    zmq::socket_t repSoc (context, ZMQ_REP);
    repSoc.bind("tcp://*:5550");

    zmq::socket_t reqSoc (context, ZMQ_REQ);
    reqSoc.bind("tcp://*:5555"); // updates from workers

    (*sqlcom).connectToDB("wmrdb", "root", "agu109");

    std::string repMessage, reqMessage, subMessage, pubMessage;
    
    int slowUpdate = 0;
    bool joined = false;

    while(true){
        if(slowUpdate > 50000 && joined){
            slowUpdate = 0;
            /* ---- Request updates from current workers ----- */
            s_send_nowait(reqSoc, "update");
            reqMessage = s_recv(reqSoc);
            std::cout << "Received new locations: " << reqMessage << std::endl;
            processRequestMessage(sqlcom, reqMessage);           
        }else{
            slowUpdate++;
        }
        
        /* ---- Reply to new worker requests and update database -----*/
        repMessage = s_recv_nowait(repSoc);
        if(!repMessage.empty()){
            joined = true;
            std::cout << "Received: " << repMessage << std::endl;
            repMessage = processReplyMessage(sqlcom, repMessage);
            std::cout << "Sending ID: " << repMessage <<std::endl;
            s_send(repSoc, repMessage);
        }
    }

    return 0;
}

void processRequestMessage(SQLAPIcoms *sqlcom, std::string reqMessage){
    std::vector<std::string> values;
    std::stringstream ss;
    std::string taskID, locX, locY;
    std::string status;
    ss << reqMessage;
    ss >> taskID >> locX >> locY >> status;
    values.push_back(taskID);
    values.push_back(locX);
    values.push_back(locY);
    values.push_back(status);
    (*sqlcom).updateWorkerStatus(values);
}


std::string processReplyMessage(SQLAPIcoms *sqlcom, std::string repMessage){

    repMessage = repMessage + " ";
                
    std::vector<std::string> values;
    std::string delimiter = " ";
    std::string command;

    size_t pos = 0;
    std::string token;

    // Get the first item in the string determined by white space
    pos = repMessage.find(delimiter);
    command = repMessage.substr(0, pos);
    repMessage.erase(0, pos + delimiter.length());

    if(!repMessage.empty()){
        // Split the remainder of the string
        while ((pos = repMessage.find(delimiter)) != std::string::npos) {
            token = repMessage.substr(0, pos);
            values.push_back(token);
            repMessage.erase(0, pos + delimiter.length());
        }

        if(command == "newJoin"){
            (*sqlcom).insertIntoWorkers(values);
            return std::to_string((*sqlcom).getNewID()); 
        }
    }

}