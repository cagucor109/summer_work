//
//  Weather update client in C++
//  Connects SUB socket to tcp://localhost:5556
//  Collects weather updates and finds avg temp in zipcode
//
#include <zmq.hpp>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include "zhelpers.hpp"

int main (int argc, char *argv[])
{
    zmq::context_t context (1);

    //  Socket to talk to server
    std::cout << "Collecting updates from task queue...\n" << std::endl;
    zmq::socket_t subscriber (context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:5556");

    //  Subscribe to topic
	const char *filter = (argc > 1)? argv [1]: "hello";
    subscriber.setsockopt(ZMQ_SUBSCRIBE, filter, strlen (filter));

    //  Process 100 updates
    int update_nbr;
    for (update_nbr = 0; update_nbr < 100; update_nbr++) {

        int taskID, locationStartX, locationEndX, weight, availability;

        std::string topic = s_recv (subscriber);
		std::string data = s_recv (subscriber);

        std::istringstream iss(data);

		iss >> taskID >> locationStartX >> locationEndX >> weight >> availability;

        printf("Received:\n taskID: %d, locationStartX: %d, locationEndX: %d, weight: %d, availability: %d \n", taskID, locationStartX, locationEndX, weight, availability);

    }

    return 0;
}
