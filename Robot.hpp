#ifndef ROBOT_HPP_
#define ROBOT_HPP_

#include <cmath>

class Robot{

private:
    int _robotID;
    int _locationX;
    int _capacity;
    int _battery;

public:
    Robot();
    int findCompatibility(int taskID, int locationStartX, int locationEndX, int weight);
};

#endif