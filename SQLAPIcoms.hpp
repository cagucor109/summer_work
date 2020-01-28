#ifndef SQLAPICOMS_HPP_
#define SQLAPICOMS_HPP_

#include <SQLAPI.h>
#include <string>
#include <vector>

class SQLAPIcoms{
    private:
        SAConnection *_con;
        SACommand *_cmd;
    public:
        SQLAPIcoms();
        void insertIntoTasks(std::vector<int> values);
        ~SQLAPIcoms();

};

#endif