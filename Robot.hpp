#ifndef ROBOT_HPP_
#define ROBOT_HPP_

#include <cmath>
#include "zhelpers.hpp"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <utility>

class Robot{
public:
    // Constructors
    Robot();
    Robot(std::string param);

    // Getters
    int getRobotID();
    int getLocationX();
    int getLocationY();
    int getCapacity();
    int getBattery();
    std::string getStatus();
    int getMaxSpeed();

    // Setters
    void setRobotID(int robotID);
    void setLocationX(int locationX);
    void setLocationY(int locationY);
    void setCapacity(int capacity);
    void setBattery(int battery);
    void setStatus(std::string status);
    void setMaxSpeed(int speed);

    // Utility
    int findCompatibility(int locationStartX, int locationStartY, int locationEndX, int locationEndY, int weight);
    void addSubscription(int taskID, int workerID, int compatibility);
    void updateSubscription(int index, int workerID, int compatibility);
    std::string checkTimeLimits();
    void updateAssignments(std::string update);
    std::string workOnAssignments();

private:
    int _totalrobots;
    int _robotID;
    int _locationX;
    int _locationY;
    int _capacity;
    int _battery;
    int _maxSpeed;
    std::string _status;
    std::vector<int> _taskIDSubscription;
    std::vector<int> _workerIDBestBid;
    std::vector<int> _compatibilityScores;
    std::vector<clock_t> _timeSinceUpdate;
    std::vector<std::string> _assignments; 
    std::vector<clock_t> _assignmentTime; 
    std::vector<int> _assignmentDist; 
    bool _reachedStart;
    bool _started;
};

#endif