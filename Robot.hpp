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
    int getRobotID();
    int getLocationX();
    int getCapacity();
    int getBattery();
    void setRobotID(int robotID);
    int findCompatibility(int taskID, int locationStartX, int locationEndX, int weight);
    void addSubscription(std::string taskID);
    std::vector<std::string> getTaskSubscriptions();
private:
    int _robotID;
    int _locationX;
    int _capacity;
    int _battery;
    std::vector<std::string> _taskSubscription;
    Robot::Status _status;
};

#endif