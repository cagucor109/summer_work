#include "SQLAPIcoms.hpp"

SQLAPIcoms::SQLAPIcoms(){

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