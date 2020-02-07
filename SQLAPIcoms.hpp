#ifndef SQLAPICOMS_HPP_
#define SQLAPICOMS_HPP_

#include <SQLAPI.h>
#include <string>
#include <vector>
#include <stdio.h>
#include <iostream>

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

        void insertIntoTasks(std::vector<int> values);
        void insertIntoWorkers(std::vector<int> values);

        int getNewID();
        ~SQLAPIcoms();

};

#endif