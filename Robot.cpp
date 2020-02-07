#include "Robot.hpp"

// Constructors
Robot::Robot(){

}

Robot::Robot(std::string param){

    if(param == "Simulation"){
        srand(time(NULL));
        _robotID = 0;
        _locationX = rand() % 10;
        _locationY = rand() % 10;
        _capacity = rand() % 3 + 2;
        _battery = rand() % 5 + 5;
    }
}

// Getters
int Robot::getRobotID(){
    return _robotID;
}

int Robot::getLocationX(){
    return _locationX;
}

int Robot::getLocationY(){
    return _locationY;
}

int Robot::getCapacity(){
    return _capacity;
}

int Robot::getBattery(){
    return _battery;
}

//Setters

void Robot::setRobotID(int robotID){
    _robotID = robotID;
}

void Robot::setLocationX(int locationX){
    _locationX = locationX;
}

void Robot::setLocationY(int locationY){
    _locationY = locationY;
}

void Robot::setCapacity(int capacity){
    _capacity = capacity;
}

void Robot::setBattery(int battery){
    _battery = battery;
}

// Utility 

// potentially change to accepting different parameters e.g. vector, single string
int Robot::findCompatibility(int locationStartX, int locationStartY, int locationEndX, int locationEndY, int weight){
    int compStartX = abs(_locationX - locationStartX);
    int compStartY = abs(_locationY - locationStartY);
    int compCap = (_capacity > weight) ? 1:0;
    int compBatt = _battery - abs(locationStartX - locationEndX) * weight; 
    return compStartX + compStartY + compCap + compBatt;
}

void Robot::addSubscription(std::string taskID){
    _taskSubscription.push_back(taskID);
}

std::vector<std::string> Robot::getTaskSubscriptions(){
    return _taskSubscription;
}