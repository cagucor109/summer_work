/*
This source file manages the connection of agv workers to the overall communication topology.
Functions:  Add a new worker into the network
            Request updates to ensure continued connection to network
Requirements:   Should have at least a request and reply socket to communicate with workers
*/

#include <iostream>
#include "SQLAPIcoms.hpp"
#include "ZMQcoms.hpp"

void newAGV();
std::string processReplyMessage(SQLAPIcoms *sqlcom, std::string repMessage);

int main(int argc, char *argv[]){
    ZMQcoms *zmqcom = new ZMQcoms();
    SQLAPIcoms *sqlcom = new SQLAPIcoms();

    std::cout << "workerHandler specific: please include at least a REQ and REP socket...\n" << std::endl;

    (*zmqcom).setUpPrompt();
    (*sqlcom).connectToDBPrompt();

    std::string repMessage, reqMessage, subMessage, pubMessage;

    while(true){
        /* ---- Request updates from current workers ----- */

        /* ---- Reply to new worker requests and update database -----*/
        repMessage = (*zmqcom).replyReceive();
        if(!repMessage.empty()){
            std::cout << "Received: " << repMessage << std::endl;
            repMessage = processReplyMessage(sqlcom, repMessage);
            (*zmqcom).replySend(repMessage);
        }
    }

    return 0;
}

void newAGV(){

}

std::string processReplyMessage(SQLAPIcoms *sqlcom, std::string repMessage){

    repMessage = repMessage + " ";
                
    std::vector<int> values;
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
            values.push_back(std::stoi(token));
            repMessage.erase(0, pos + delimiter.length());
        }

        if(command == "newJoin"){
            (*sqlcom).insertIntoWorkers(values);
            return std::to_string((*sqlcom).getNewID()); 
        }
    }

}