#include "SQLAPIcoms.hpp"

SQLAPIcoms::SQLAPIcoms(){
    _con = new SAConnection();
    _cmd = new SACommand();
}

void SQLAPIcoms::connectToDBPrompt(){
    printf("\nSQL set up prompt initiated...\n");
    printf("\nEnter the database name, user and password...\n");
    std::string dB, user, pwd;
    printf("Data base name:\t");
    std::cin >> dB;
    printf("User name:\t");
    std::cin >> user;
    printf("Password name:\t");
    std::cin >> pwd;
    connectToDB(dB, user, pwd);
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
            "INSERT INTO tasks(locationStartX, locationStartY, locationEndX, locationEndY, weight) VALUES(:1, :2, :3, :4, :5)");
    
    for(int i = 0; i < values.size(); i++){
        (*_cmd).Param(i+1).setAsInt64() = values.at(i);
    }
   
    (*_cmd).Execute();
    
    (*_con).Commit();
}

void SQLAPIcoms::insertIntoWorkers(std::vector<int> values){
    (*_cmd).setCommandText(
            "INSERT INTO workers(locationX, locationY, capacity, battery) VALUES(:1, :2, :3, :4)");
    
    for(int i = 0; i < values.size(); i++){
        (*_cmd).Param(i+1).setAsInt64() = values.at(i);
    }
   
    (*_cmd).Execute();
    
    (*_con).Commit();
}

int SQLAPIcoms::getNewID(){
    int newID;

    (*_cmd).setCommandText(
        "SELECT LAST_INSERT_ID()");

    (*_cmd).Execute();
    (*_cmd).FetchFirst();
    newID = (*_cmd).Field("LAST_INSERT_ID()").asInt64();
    return newID;
}

SQLAPIcoms::~SQLAPIcoms(){
    delete _con;
    delete _cmd;
}