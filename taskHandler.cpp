#include "ZMQcoms.hpp"
#include "Robot.hpp"
#include "SQLAPIcoms.hpp"
#include <iostream>

std::string createRandomTask(ZMQcoms *com);
void updateTasksTable(std::string task);

int main(int argc, char *argv[]){

    ZMQcoms *zmqcom = new ZMQcoms();
    SQLAPIcoms *sqlcom = new SQLAPIcoms();
    Robot *agv = new Robot();

    std::cout << "This instance launches PUB and SUB sockets by default" << std::endl;
    std::cout << "The newTask topic is reserved for new tasks" << std::endl;
    std::cout << "The acceptTask topic is subscribed to by default" << std::endl;

    int port;

    std::cout << "\nEnter the port to bind the PUB socket..." << std::endl;
    std::cin >> port;
    std::cin.ignore();
    (*zmqcom).setup(ZMQcoms::PUB, port);

    std::cout << "\nEnter the port to connect the SUB socket..." << std::endl;
    std::cin >> port;
    std::cin.ignore();
    (*zmqcom).setup(ZMQcoms::SUB, port);
    (*zmqcom).subscribeToTopic("acceptTask");

    std::cout << "Setup success!" << std::endl;

    while(true){
        std::string newTask;
        newTask = createRandomTask(zmqcom);
        updateTasksTable(newTask);
        sleep(3);

    }

    return 0;
}

std::string createRandomTask(ZMQcoms *com){

    int taskID, locationStartX, locationEndX, weight, availability;

    //  Get values that will fool the boss
    taskID          = within (100);
    locationStartX  = within (10);
    locationEndX    = within (10);
    weight          = within (5);
    availability    = 0;

    std::stringstream ss;
    ss << taskID << " " << locationStartX  << " " << locationEndX  << " " << weight  << " " << availability;

    printf("Sending taskID: %d, locationStartX: %d, locationEndX: %d, weight: %d, availability: %d \n", taskID, locationStartX, locationEndX, weight, availability);
    
     //  Send message to all subscribers
    (*com).publishMessage("newTask", ss.str());

    return ss.str();
}

void updateTasksTable(std::string task){
    if(!task.empty()){
        std::stringstream ss;
        int taskID, locationStartX, locationEndX, weight, availability;
        ss << task;
        ss >> taskID >> locationStartX >> locationEndX >> weight >> availability;
        std::cout << "Updating table with: " << std::endl;
        std::cout << taskID << " " << locationStartX  << " " << locationEndX  << " " << weight  << " " << availability << std::endl;
    
    }
}