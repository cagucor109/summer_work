#ifndef SQLAPICOMS_HPP_
#define SQLAPICOMS_HPP_

#include <SQLAPI.h>
#include <string>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <sstream>

class SQLAPIcoms{
    private:
        SAConnection *_con;
        SACommand *_cmd;
    public:
        // constructors
        SQLAPIcoms();

        // connect
        void connectToDBPrompt();
        void connectToDB(std::string dB, std::string user, std::string pwd);

        void insertIntoTasks(std::vector<std::string> values);
        void insertIntoWorkers(std::vector<std::string> values);
        void insertIntoAssignments(std::vector<std::string> values);

        void updateWorkerStatus(std::vector<std::string> values);
        void updateAssignmentStatus(std::vector<std::string> values);
        void updateAssignmentTimeDist(std::vector<std::string> values);

        std::string getCoordinates(int taskID);
        bool checkIfTaken(int taskID);

        int getNewID();
        ~SQLAPIcoms();

};

#endif