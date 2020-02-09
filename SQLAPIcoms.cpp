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

void SQLAPIcoms::insertIntoTasks(std::vector<std::string> values){
    (*_cmd).setCommandText(
            "INSERT INTO tasks(locationStartX, locationStartY, locationEndX, locationEndY, weight) VALUES(:1, :2, :3, :4, :5)");
    
    (*_cmd).Param(1).setAsInt64() = std::stoi(values.at(0));
    (*_cmd).Param(2).setAsInt64() = std::stoi(values.at(1));
    (*_cmd).Param(3).setAsInt64() = std::stoi(values.at(2));
    (*_cmd).Param(4).setAsInt64() = std::stoi(values.at(3));
    (*_cmd).Param(5).setAsInt64() = std::stoi(values.at(4));
   
    (*_cmd).Execute();
    
    (*_con).Commit();
}

void SQLAPIcoms::insertIntoWorkers(std::vector<std::string> values){
    (*_cmd).setCommandText(
            "INSERT INTO workers(locationX, locationY, capacity, battery, maxSpeed, status) VALUES(:1, :2, :3, :4, :5, :6)");
    
    
    (*_cmd).Param(1).setAsInt64() = std::stoi(values.at(0));
    (*_cmd).Param(2).setAsInt64() = std::stoi(values.at(1));
    (*_cmd).Param(3).setAsInt64() = std::stoi(values.at(2));
    (*_cmd).Param(4).setAsInt64() = std::stoi(values.at(3));
    (*_cmd).Param(5).setAsInt64() = std::stoi(values.at(4));
    (*_cmd).Param(6).setAsString() = values.at(5).c_str();
   
    (*_cmd).Execute();
    
    (*_con).Commit();
}

void SQLAPIcoms::insertIntoAssignments(std::vector<std::string> values){
    (*_cmd).setCommandText(
            "INSERT INTO assignments(taskID, robotID, status, time) VALUES(:1, :2, :3, :4)");
    
    (*_cmd).Param(1).setAsInt64() = std::stoi(values.at(0));
    (*_cmd).Param(2).setAsInt64() = std::stoi(values.at(1));
    (*_cmd).Param(3).setAsString() = "assigned";
    (*_cmd).Param(4).setAsInt64() = 0;

    (*_cmd).Execute();
    
    (*_con).Commit();
}

std::string SQLAPIcoms::getCoordinates(int taskID){
    (*_cmd).setCommandText(
            "SELECT locationStartX, locationStartY, locationEndX, locationEndY FROM tasks WHERE taskID = :1");

    (*_cmd).Param(1).setAsInt64() = taskID;

    (*_cmd).Execute();
    (*_cmd).FetchFirst();
    int sX = (*_cmd).Field("locationStartX").asInt64();
    int sY = (*_cmd).Field("locationStartY").asInt64();
    int eX = (*_cmd).Field("locationEndX").asInt64();
    int eY = (*_cmd).Field("locationEndY").asInt64();

    std::stringstream ss;
    ss << sX << " " << sY << " " << eX << " " << eY;

    return ss.str();
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