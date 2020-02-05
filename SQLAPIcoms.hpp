#ifndef SQLAPICOMS_HPP_
#define SQLAPICOMS_HPP_

#include <SQLAPI.h>
#include <string>
#include <vector>
#include <stdio.h>

class SQLAPIcoms{
    private:
        SAConnection *_con;
        SACommand *_cmd;
    public:
        SQLAPIcoms();
        void connectToDB(std::string dB, std::string user, std::string pwd);
        void insertIntoTasks(std::vector<int> values);
        ~SQLAPIcoms();

};

#endif