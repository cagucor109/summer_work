#include "ZMQcoms.hpp"
#include "SQLAPIcoms.hpp"
#include "Robot.hpp"
#include <iostream>

void pubInterface(ZMQcoms *zmqcom);
void subInterface(ZMQcoms *zmqcom);
void reqInterface(ZMQcoms *zmqcom);
void repInterface(ZMQcoms *zmqcom);

int main(int argc, char *argv[]){

    ZMQcoms *zmqcom = new ZMQcoms();
    SQLAPIcoms *sqlcom = new SQLAPIcoms();

    std::cout << "Enter the number corresponding to the desired type of socket..." << std::endl;
    std::cout << "1: PUB\n2: SUB\n3: REQ\n4: REP" << std::endl;

    int type;
    std::cin >> type;

    ZMQcoms::Pattern pattern = (ZMQcoms::Pattern)type;

    std::cout << "Enter the port to connect to..." << std::endl;

    int port;
    std::cin >> port;

    (*zmqcom).setup(pattern, port);

    while(true){
        if(pattern == ZMQcoms::PUB){
            pubInterface(zmqcom);
        }else if(pattern == ZMQcoms::SUB){
            subInterface(zmqcom);
        } else{

        }
    }

    return 0;
}

// functions change user interface based on user input
void pubInterface(ZMQcoms *zmqcom){
    std::string topic, message;
    std::cout << "\nTo publish a message:\nFirst enter the topic to publish to\nThen enter the message " << std::endl;
    std::cout << "\nWaiting for topic..." << std::endl;
    std::cin >> topic;
    std::cin.ignore();
    std::cout << "\nWaiting for message..." << std::endl;
    std::getline(std::cin, message);
    std::cout << "\nPublishing:\n\t" << message << "\nTo topic:\n\t" << topic << std::endl;
    (*zmqcom).publishMessage(topic, message);
}

void subInterface(ZMQcoms *zmqcom){
    std::string topic;
    std::cout << "\nTo subscribe to messages from a topic:\nFirst enter a topics to subscribe to\nThen receive the messages... " << std::endl;
    std::cout << "Waiting for topic..." << std::endl;
    std::cin >> topic;
    (*zmqcom).subscribeToTopic(topic);
    std::cout << "\nReceived: " << (*zmqcom).subscribeMessage() << std::endl;
}

void reqInterface(ZMQcoms &zmqcom){

}

void repInterface(ZMQcoms &zmqcom){

}