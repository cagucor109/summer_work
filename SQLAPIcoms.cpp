#include "SQLAPIcoms.hpp"

SQLAPIcoms::SQLAPIcoms(){
    _con = new SAConnection();
    _cmd = new SACommand();
}

void SQLAPIcoms::connectToDB(std::string dB, std::string user, std::string pwd){
    SAString dBname = dB.c_str();
    SAString username = user.c_str();
    SAString password = pwd.c_str();
    (*_con).Connect(dBname, username, password, SA_MySQL_Client);
    printf("\nWe are connected to database!\n");
    (*_cmd).setConnection(_con);
}

void SQLAPIcoms::insertIntoTasks(std::vector<int> values){
    (*_cmd).setCommandText(
            "INSERT INTO tasks VALUES(:1, :2, :3, :4, :5)");
    
    for(int i = 0; i < values.size(); i++){
        (*_cmd).Param(i+1).setAsInt64() = values.at(i);
    }
   
    (*_cmd).Execute();
    
    (*_con).Commit();
}

SQLAPIcoms::~SQLAPIcoms(){
    delete _con;
    delete _cmd;
}