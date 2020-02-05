#include "ZMQcoms.hpp"
#include "Robot.hpp"
#include "SQLAPIcoms.hpp"
#include <iostream>

std::string createRandomTask(ZMQcoms *com);
void updateTasksTable(SQLAPIcoms *com, std::string task);

int main(int argc, char *argv[]){

    ZMQcoms *zmqcom = new ZMQcoms();
    SQLAPIcoms *sqlcom = new SQLAPIcoms();
    Robot *agv = new Robot();

    std::cout << "This instance launches PUB and SUB sockets by default" << std::endl;
    std::cout << "The newTask topic is reserved for new tasks" << std::endl;
    std::cout << "The acceptTask topic is subscribed to by default" << std::endl;

    int port;
    std::string bind_con;

    std::cout << "\nEnter the port for the PUB socket..." << std::endl;
    std::cin >> port;
    std::cin.ignore();
    std::cout << "\nDo you want to connect or bind?" << std::endl;
    std::cin >> bind_con;
    (*zmqcom).setUp(ZMQcoms::PUB);
    (*zmqcom).setConnection(ZMQcoms::PUB, port, bind_con);

    std::cout << "\nEnter the port for the SUB socket..." << std::endl;
    std::cin >> port;
    std::cin.ignore();
    std::cout << "\nDo you want to connect or bind?" << std::endl;
    std::cin >> bind_con;
    (*zmqcom).setUp(ZMQcoms::SUB);
    (*zmqcom).setConnection(ZMQcoms::SUB, port, bind_con);
    (*zmqcom).subscribeToTopic("acceptTask");

    std::string dBName;
    std::string user;
    std::string pwd;

    std::cout <<"Enter database name to connect to..." << std::endl;
    std::cin >> dBName;
    std::cout <<"Enter username to connect with..." << std::endl;
    std::cin >> user;
    std::cout <<"Enter password..." << std::endl;
    std::cin >> pwd;

    try{

        (*sqlcom).connectToDB(dBName, user, pwd);

        std::cout << "Setup success!" << std::endl;

        while(true){
            std::string newTask;
            newTask = createRandomTask(zmqcom);
            updateTasksTable(sqlcom, newTask);
            sleep(5);

        }

    }
    catch(SAException &x){
        printf("%s\n", (const char*)x.ErrText());
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

void updateTasksTable(SQLAPIcoms *com, std::string task){
    if(!task.empty()){
        std::vector<int> tableUpdate;
        std::stringstream ss;
        int taskID, locationStartX, locationEndX, weight, availability;
        ss << task;
        ss >> taskID >> locationStartX >> locationEndX >> weight >> availability;
        tableUpdate.push_back(taskID);
        tableUpdate.push_back(locationStartX);
        tableUpdate.push_back(locationEndX);
        tableUpdate.push_back(weight);
        tableUpdate.push_back(availability);
        std::cout << "Updating table with: " << std::endl;
        std::cout << taskID << " " << locationStartX  << " " << locationEndX  << " " << weight  << " " << availability << std::endl;

        (*com).insertIntoTasks(tableUpdate);

    
    }
}