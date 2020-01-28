#include "Robot.hpp"

Robot::Robot(){

}

int Robot::findCompatibility(int taskID, int locationStartX, int locationEndX, int weight){
    int compStart = abs(_locationX - locationStartX);
    int compCap = (_capacity > weight) ? 1:0;
    int compBatt = _battery - abs(locationStartX - locationEndX) * weight; 
}