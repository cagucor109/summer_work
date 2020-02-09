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

void newAGV();
std::string processReplyMessage(SQLAPIcoms *sqlcom, std::string repMessage);

int main(int argc, char *argv[]){
    SQLAPIcoms *sqlcom = new SQLAPIcoms();

    zmq::context_t context (1);
    
    zmq::socket_t repSoc (context, ZMQ_REP);
    repSoc.bind("tcp://*:5550");

    (*sqlcom).connectToDB("wmrdb", "root", "agu109");

    std::string repMessage, reqMessage, subMessage, pubMessage;

    while(true){
        /* ---- Request updates from current workers ----- */

        /* ---- Reply to new worker requests and update database -----*/
        repMessage = s_recv_nowait(repSoc);
        if(!repMessage.empty()){
            std::cout << "Received: " << repMessage << std::endl;
            repMessage = processReplyMessage(sqlcom, repMessage);
            std::cout << "Sending ID: " << repMessage <<std::endl;
            s_send(repSoc, repMessage);
        }
    }

    return 0;
}

void newAGV(){

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