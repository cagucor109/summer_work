#include "ZMQcoms.hpp"
#include "SQLAPIcoms.hpp"
#include <iostream>

std::string processReplyMessage(SQLAPIcoms *sqlcom, std::string message);

int main(int argc, char *argv[]){
    SQLAPIcoms *sqlcom = new SQLAPIcoms();

    zmq::context_t context (1);
    
    zmq::socket_t repSoc (context, ZMQ_REP);
    repSoc.bind("tcp://*:5554");

    (*sqlcom).connectToDB("wmrdb", "root", "agu109");

    std::string repMessage;

    while(true){
        repMessage = s_recv_nowait(repSoc);
        if(!repMessage.empty()){
            std::cout << "Received: " << repMessage << std::endl;
            repMessage = processReplyMessage(sqlcom, repMessage);
            std::cout << "Sending coordinates: " << repMessage <<std::endl;
            s_send_nowait(repSoc, repMessage);
        }
    }

    return 0;
}

std::string processReplyMessage(SQLAPIcoms *sqlcom, std::string message){
    std::stringstream ss;
    std::vector<std::string> values;
    std::string taskID, workerID;
    ss << message;
    ss >> taskID >> workerID;
    values.push_back(taskID);
    values.push_back(workerID);
    (*sqlcom).insertIntoAssignments(values);

    // update status of worker
    
    return (*sqlcom).getCoordinates(std::stoi(taskID));
}