#include "Robot.hpp"

int Robot::_robotCount = 0;

Robot::Robot(){
    _robotID = _robotCount++;
    _locationX = within(10);
    _capacity = within(5);
    _battery = within(100);

}

int Robot::findCompatibility(int taskID, int locationStartX, int locationEndX, int weight){
    int compStart = abs(_locationX - locationStartX);
    int compCap = (_capacity > weight) ? 1:0;
    int compBatt = _battery - abs(locationStartX - locationEndX) * weight; 
    return compStart + compCap + compBatt;
}