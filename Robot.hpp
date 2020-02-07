#ifndef ROBOT_HPP_
#define ROBOT_HPP_

#include <cmath>
#include "zhelpers.hpp"
#include <cstdlib>
#include <ctime>

class Robot{
public:
    enum Status{
        IDLE,
        PROCESSING,
        ASSIGNED
    };

    // Constructors
    Robot();
    Robot(std::string param);

    // Getters
    int getRobotID();
    int getLocationX();
    int getLocationY();
    int getCapacity();
    int getBattery();

    // Setters
    void setRobotID(int robotID);
    void setLocationX(int locationX);
    void setLocationY(int locationY);
    void setCapacity(int capacity);
    void setBattery(int battery);

    // Utility
    int findCompatibility(int locationStartX, int locationStartY, int locationEndX, int locationEndY, int weight);
    void addSubscription(std::string taskID);
    std::vector<std::string> getTaskSubscriptions();
private:
    int _robotID;
    int _locationX;
    int _locationY;
    int _capacity;
    int _battery;
    std::vector<std::string> _taskSubscription;
    Robot::Status _status;
};

#endif