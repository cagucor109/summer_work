#include "Robot.hpp"

// Constructors
Robot::Robot(){

}

Robot::Robot(std::string param){

    if(param == "Simulation"){
        srand(time(NULL));
        _robotID = 0;
        _locationX = rand() % 11;
        _locationY = rand() % 11;
        _capacity = rand() % 4 + 2;
        _battery = rand() % 6 + 5;
        _status = "idle";
        _maxSpeed = rand() % 5 + 2;
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

std::string Robot::getStatus(){
    return _status;
}

int Robot::getMaxSpeed(){
    return _maxSpeed;
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

void Robot::setStatus(std::string status){
    _status = status;
}

void Robot::setMaxSpeed(int speed){
    _maxSpeed = speed;
}

// Utility 

// potentially change to accepting different parameters e.g. vector, single string
int Robot::findCompatibility(int locationStartX, int locationStartY, int locationEndX, int locationEndY, int weight){
    int compStartX = abs(_locationX - locationStartX);
    int compStartY = abs(_locationY - locationStartY);
    int compCap = (_capacity > weight) ? 1:0;
    float compBatt = _battery - abs(locationStartX - locationEndX) * weight / 5;
    float compSpeed = (pow((locationEndX - locationStartX), 2) + pow(locationEndY - locationStartY, 2)) / (pow(_maxSpeed, 2) * 10);
    return compStartX + compStartY + compCap + compBatt - compSpeed;
}

void Robot::addSubscription(int taskID, int workerID, int compatibility){
    std::vector<int>::iterator it = std::find(_taskIDSubscription.begin(), _taskIDSubscription.end(), taskID);
    // bid not currently in system
    if(it == _taskIDSubscription.end()){
        clock_t t;
        t = clock();
        _timeSinceUpdate.push_back(t);
        _taskIDSubscription.push_back(taskID);
        _workerIDBestBid.push_back(workerID);
        _compatibilityScores.push_back(compatibility);
    }else{
        int index = std::distance(_taskIDSubscription.begin(), it);
        if(_compatibilityScores.at(index) < compatibility){
            updateSubscription(index, workerID, compatibility);
        }else if(_compatibilityScores.at(index) == compatibility && workerID == _robotID){
            // this is done so the following doesn't happen:
            // multiple workers have some compatibility 
            // they all catalogue another worker as most compatible
            // nothing gets done
            updateSubscription(index, workerID, compatibility);
        }
    }
    
}

void Robot::updateSubscription(int index, int workerID, int compatibility){
    clock_t t;
    t = clock();
    _timeSinceUpdate.at(index) = t;
    _workerIDBestBid.at(index) = workerID;
    _compatibilityScores.at(index) = compatibility;
}

//go through all subscribed tasks and check if time since last update
//is greater than x seconds (6) and finish task bidding stage
//if the best bid is the current bid, then add to assignments
std::string Robot::checkTimeLimits(){
    clock_t t;
    std::string out = "";
    int elapasedTime;
    for(int i = 0; i < _taskIDSubscription.size(); i++){
        t = clock() -  _timeSinceUpdate.at(i);
        elapasedTime = (int)t/CLOCKS_PER_SEC;
        if(elapasedTime > 6){
            std::cout << "Biding closed for taskID: " << _taskIDSubscription.at(i) << std::endl;
            std::cout << "Winner is: " << _workerIDBestBid.at(i) << std::endl;
            if(_workerIDBestBid.at(i) ==  _robotID){
                out = std::to_string(_taskIDSubscription.at(i)) + " " + std::to_string(_robotID);
            }
            _taskIDSubscription.erase(_taskIDSubscription.begin() + i);
            _workerIDBestBid.erase(_workerIDBestBid.begin() + i);
            _compatibilityScores.erase(_compatibilityScores.begin() + i);
            _timeSinceUpdate.erase(_timeSinceUpdate.begin() + i);
        }
    }
    
    return out;

}