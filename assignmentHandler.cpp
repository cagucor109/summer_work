#include "ZMQcoms.hpp"
#include "SQLAPIcoms.hpp"
#include <iostream>
#include <cctype>

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
            std::cout << "sending: " << repMessage << std::endl; 
            s_send_nowait(repSoc, repMessage);
        }
    }

    return 0;
}

std::string processReplyMessage(SQLAPIcoms *sqlcom, std::string message){
    std::stringstream ss;
    std::vector<std::string> values;
    std::string first, second;
    std::string taskID, workerID;
    std::string status;
    std::string time, distance;
    ss << message;
    ss >> first;
    if(isdigit(first.at(0))){
        taskID = first;
        ss >> workerID;
        values.push_back(taskID);
        values.push_back(workerID);
        (*sqlcom).insertIntoAssignments(values);

        return taskID + " " + (*sqlcom).getCoordinates(std::stoi(taskID));
    }else{
        status = first;
        ss >> taskID >> workerID;

        values.push_back(taskID);
        values.push_back(workerID);
        values.push_back(status);

        (*sqlcom).updateAssignmentStatus(values);

        values.pop_back(); // remove status

        if(status == "complete"){
            ss >> time >> distance;
            values.push_back(time);
            values.push_back(distance);
            (*sqlcom).updateAssignmentTimeDist(values);
        }

        return "updated";
    }
    
}