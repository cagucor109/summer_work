#include "Robot.hpp"

// Constructors
Robot::Robot(){

}

Robot::Robot(std::string param){

    if(param == "Simulation"){
        srand(time(NULL));
        _robotID = 0;
        _locationX = rand() % 101;
        _locationY = rand() % 101;
        _capacity = rand() % 4 + 2;
        _battery = rand() % 6 + 5;
        _status = "idle";
        _maxSpeed = rand() % 3 + 1;
        _reachedStart = false;
        _started = false;
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
    int compStartX = abs(_locationX - locationStartX) * 2;
    int compStartY = abs(_locationY - locationStartY) * 2;
    int compCap = (_capacity > weight) ? 1:0;
    float compBatt = _battery - abs(locationStartX - locationEndX) * weight / 5;
    float compSpeed = (pow((locationEndX - locationStartX), 2) + pow(locationEndY - locationStartY, 2)) / (pow(_maxSpeed, 2) * 200);
    int compQueue = _assignments.size();
    return compStartX + compStartY + compCap + compBatt - compSpeed - compQueue;
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
//is greater than x seconds (4) and finish task bidding stage
//if the best bid is the current bid, then add to assignments
std::string Robot::checkTimeLimits(){
    clock_t t;
    std::string out = "";
    int elapasedTime;
    for(int i = 0; i < _taskIDSubscription.size(); i++){
        t = clock() -  _timeSinceUpdate.at(i);
        elapasedTime = (int)t/CLOCKS_PER_SEC;
        if(elapasedTime > 4){
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

// taskID, sX, sY, eX, eY
void Robot::updateAssignments(std::string update){
    clock_t time = clock();
    _assignments.push_back(update);
    _assignmentTime.push_back(time);
    _assignmentDist.push_back(0);
}

std::string Robot::workOnAssignments(){
    std::stringstream ss;
    clock_t time = clock();
    int xdist, ydist;
    int xmotion, ymotion; 
    float angle;
    int taskID, sX, sY, eX, eY;
    int overShootX, overShootY;

    int finalDist, finalTime;

    if(_assignments.size() > 0){
        // to update time and dist columns in db
        if(!_started){
            _started = true;
            _assignmentTime.at(0) = time;
            _assignmentDist.at(0) = 0;
        }
        // change status
        _status = "working";
        ss << _assignments.at(0);
        ss >> taskID >> sX >> sY >> eX >> eY;
        // move to start coordinates
        if(!_reachedStart){
            xdist = sX - _locationX;
            ydist = sY - _locationY;

            if(xdist != 0){
                angle = atan(abs(ydist)/abs(xdist));
                xmotion = ceil(_maxSpeed * cos(angle));
            }else{
                angle = 1.57;
                xmotion = 0;
            }
            ymotion = ceil(_maxSpeed * sin(angle));

            xmotion = (xdist > 0) ? xmotion: xmotion*-1;
            ymotion = (ydist > 0) ? ymotion: ymotion*-1;

            _assignmentDist.at(0) += abs(xmotion) + abs(ymotion);

            _locationX += xmotion;
            _locationY += ymotion;

            overShootX = sX - _locationX;
            overShootY = sY - _locationY;

            if(overShootX * xdist < 0){
                _locationX = sX;
                _assignmentDist.at(0) -= abs(overShootX);
            }
             if(overShootY * ydist < 0){
                _locationY = sY;
                _assignmentDist.at(0) -= abs(overShootY);
            }
            
            //arrived at start
            if(_locationX == sX && _locationY == sY){
                _reachedStart = true;
                return "in-progress " + std::to_string(taskID) + " " + std::to_string(_robotID);
            }
        }
        // move to end coordinates
        else{
            xdist = eX - _locationX;
            ydist = eY - _locationY;

            if(xdist != 0){
                angle = atan(abs(ydist)/abs(xdist));
                xmotion = ceil(_maxSpeed * cos(angle));
            }else{
                angle = 1.57;
                xmotion = 0;
            }
            ymotion = ceil(_maxSpeed * sin(angle));

            xmotion = (xdist > 0) ? xmotion: xmotion*-1;
            ymotion = (ydist > 0) ? ymotion: ymotion*-1;

            _assignmentDist.at(0) += abs(xmotion) + abs(ymotion);
          
            _locationX += xmotion;
            _locationY += ymotion;

            overShootX = eX - _locationX;
            overShootY = eY - _locationY;

            if(overShootX * xdist < 0){
                _locationX = eX;
                _assignmentDist.at(0) -= abs(overShootX);
            }
             if(overShootY * ydist < 0){
                _locationY = eY;
                _assignmentDist.at(0) -= abs(overShootY);
            }

            //arrived at finish
            if(_locationX == eX && _locationY == eY){
                _started = false;
                _reachedStart = false;
                _status = "idle";
                finalDist = _assignmentDist.at(0);
                finalTime = (int)(clock() - _assignmentTime.at(0))/CLOCKS_PER_SEC;

                _assignmentTime.erase(_assignmentTime.begin());
                _assignmentDist.erase(_assignmentDist.begin());
                _assignments.erase(_assignments.begin());
                return "complete " + std::to_string(taskID) + " " + std::to_string(_robotID) + " " + std::to_string(finalTime) + " " + std::to_string(finalDist);
            }
        }
    }
    return "";
}