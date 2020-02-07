#include "ZMQcoms.hpp"
#include "Robot.hpp"
#include "SQLAPIcoms.hpp"
#include <iostream>

std::string createTask(ZMQcoms *com, std::string mode);
std::string updateTasksTable(SQLAPIcoms *com, std::string task);

int main(int argc, char *argv[]){

    srand(time(NULL));

    ZMQcoms *zmqcom = new ZMQcoms();
    SQLAPIcoms *sqlcom = new SQLAPIcoms();
    Robot *agv = new Robot();

    (*zmqcom).setUpPrompt();
    (*sqlcom).connectToDBPrompt();

    std::string mode;

    std::cout << "Do you want to manually, or automatically generate new tasks?" << std::endl;
    std::cin >> mode;

    std::string newTask, newTaskID;
    while(true){
        newTask = createTask(zmqcom, mode);
        newTaskID = updateTasksTable(sqlcom, newTask);
        (*zmqcom).publishMessage("newTask", newTaskID + " " + newTask);
        std::cout << "Sending: newTask " << newTaskID + " " + newTask << std::endl;
        sleep(5);
    }
    

    return 0;
}

std::string createTask(ZMQcoms *com, std::string mode){
    std::stringstream ss;
    int locationStartX, locationStartY, locationEndX, locationEndY, weight;
    if(mode == "manual"){
        std::cout << "Enter the value for the following fields..." << std::endl;
        printf("X Start location(0 - 9):\t");
        std::cin >> locationStartX;
        printf("Y Start location(0 - 9):\t");
        std::cin >> locationStartY;
        printf("X End location(0 - 9):\t");
        std::cin >> locationEndX;
        printf("Y End location(0 - 9):\t");
        std::cin >> locationEndY;
        printf("Weight(2 - 5):\t");
        std::cin >> weight;
    }else if(mode == "automatic"){
        locationStartX = rand() % 10;
        locationStartY = rand() % 10;
        locationEndX = rand() % 10;
        locationEndY = rand() % 10;
        weight = rand() % 3 + 2;
    }else{
        // TODO: ADD ERROR HANDLING HERE
    }
    ss << locationStartX << " " << locationStartY << " " << locationEndX << " "
        << locationEndY << " " << weight;

    return ss.str();

}

std::string updateTasksTable(SQLAPIcoms *sqlcom, std::string task){

    task = task + " ";
                
    std::vector<int> values;
    std::string delimiter = " ";

    size_t pos = 0;
    std::string token;

    if(!task.empty()){
        // Split the remainder of the string
        while ((pos = task.find(delimiter)) != std::string::npos) {
            token = task.substr(0, pos);
            values.push_back(std::stoi(token));
            task.erase(0, pos + delimiter.length());
        }

        (*sqlcom).insertIntoTasks(values);

        return std::to_string((*sqlcom).getNewID()); 
    }

}