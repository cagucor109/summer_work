#ifndef ROBOT_HPP_
#define ROBOT_HPP_

#include <cmath>
#include "zhelpers.hpp"

class Robot{
public:
    enum Status{
        IDLE,
        PROCESSING,
        ASSIGNED
    };
    Robot();
    int findCompatibility(int taskID, int locationStartX, int locationEndX, int weight);

private:
    static int _robotCount;
    int _robotID;
    int _locationX;
    int _capacity;
    int _battery;
    Robot::Status _status;
};

#endif