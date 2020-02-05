#include "Robot.hpp"

Robot::Robot(){
    _locationX = within(10);
    _capacity = within(5);
    _battery = within(100);

}

int Robot::getRobotID(){
    return _robotID;
}

int Robot::getLocationX(){
    return _locationX;
}

int Robot::getCapacity(){
    return _capacity;
}

int Robot::getBattery(){
    return _battery;
}

void Robot::setRobotID(int robotID){
    _robotID = robotID;
}

int Robot::findCompatibility(int taskID, int locationStartX, int locationEndX, int weight){
    int compStart = abs(_locationX - locationStartX);
    int compCap = (_capacity > weight) ? 1:0;
    int compBatt = _battery - abs(locationStartX - locationEndX) * weight; 
    return compStart + compCap + compBatt;
}

void Robot::addSubscription(std::string taskID){
    _taskSubscription.push_back(taskID);
}

std::vector<std::string> Robot::getTaskSubscriptions(){
    return _taskSubscription;
}