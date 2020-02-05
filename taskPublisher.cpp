//
//  Weather update server in C++
//  Binds PUB socket to tcp://*:5556
//  Publishes new tasks and updates table in database
//
#include <zmq.hpp>
#include <stdio.h>
#include <SQLAPI.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "zhelpers.hpp"

#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>

#define sleep(n)	Sleep(n)
#endif

#if (defined (WIN32))
#include <zhelpers.hpp>
#endif

int main () {

    SAConnection con;
    SACommand cmd;

    //  Prepare our context and publisher
    zmq::context_t context (1);
    zmq::socket_t publisher (context, ZMQ_PUB);
    publisher.bind("tcp://*:5556");

    // Initiate variables and constants
    int n;
    std::string TOPIC = "hello"; // <= Change your topic message here
    std::string message;

    std::cout << "How do you want to use the task publisher?" << std::endl;
    std::cout << "[n]: generates and publishes n random tasks" << std::endl;
    std::cin >> n;
 
    try {
        con.Connect("wmrdb", "root", "agu109", SA_MySQL_Client);
        printf("\nWe are connected!\n");
        cmd.setConnection(&con);

        //  Initialize random number generator
        srandom ((unsigned) time (NULL));
        for(int i = 0; i < n; i++) {

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
            
            // Insert data into databse
            cmd.setCommandText(
            "INSERT INTO tasks VALUES(:1, :2, :3, :4, :5)");
            cmd.Param(1).setAsInt64() = taskID;
            cmd.Param(2).setAsInt64() = locationStartX;
            cmd.Param(3).setAsInt64() = locationEndX;
            cmd.Param(4).setAsInt64() = weight;
            cmd.Param(5).setAsInt64() = availability;
            cmd.Execute();
    
            con.Commit();

            //  Send message to all subscribers
            //s_sendmore (publisher, TOPIC);
            //s_send (publisher, ss.str());

            sleep(1);
        }

        con.Disconnect();
        printf("We are disconnected!\n");
    }
    catch(SAException &x) {
        con.Rollback();
        printf("%s\n", (const char*)x.ErrText());
    }

    return 0;
}
