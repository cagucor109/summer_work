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
    std::string bind_con;
    std::cin >> type;

    ZMQcoms::Pattern pattern = (ZMQcoms::Pattern)type;

    std::cout << "Enter the port to connect to..." << std::endl;

    int port;
    std::cin >> port;
    std::cin.ignore();
    std::cout << "\nDo you want to connect or bind?" << std::endl;
    std::cin >> bind_con;

    (*zmqcom).setup(pattern, port, bind_con);
    std::cout << "Setup success!" << std::endl;

    if(pattern == ZMQcoms::PUB){
        pubInterface(zmqcom);
    }else if(pattern == ZMQcoms::SUB){
        subInterface(zmqcom);
    }else if(pattern == ZMQcoms::REQ){
        reqInterface(zmqcom);
    } else if(pattern == ZMQcoms::REP){
        repInterface(zmqcom);
    } else{
        std::cout << "Invalid input, please run again" << std::endl;
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
    while(true){
        std::cout << "\nWaiting for message..." << std::endl;
        std::getline(std::cin, message);
        std::cout << "\nPublishing:\n\t" << message << "\nTo topic:\n\t" << topic << std::endl;
        (*zmqcom).publishMessage(topic, message);
    }
}

void subInterface(ZMQcoms *zmqcom){
    std::string topic, message;
    std::cout << "\nTo subscribe to messages from a topic:\nFirst enter a topics to subscribe to\nThen receive the messages... " << std::endl;
    std::cout << "Waiting for topic..." << std::endl;
    std::cin >> topic;
    (*zmqcom).subscribeToTopic(topic);
    std::cout << "Successfully subscribed to " << topic << std::endl;
    while(true){
        message = (*zmqcom).subscribeMessage().at(1);
        sleep(2);
        // ..... Do work ..... //
        if(!message.empty()){
            std::cout << "\nReceived: " << message << std::endl;
        }
    }
}

void reqInterface(ZMQcoms *zmqcom){
    std::string request, reply;
    std::cout << "\nTo send a request, simply enter a message...\n" << std::endl;
    while(true){
        std::cout << "\nWaiting for request message...\n" << std::endl;
        std::getline(std::cin, request);
        if( (*zmqcom).requestSend(request) ){
            std::cout << "Sent: " << request << std::endl;
            reply = (*zmqcom).requestReceive();
            if(!reply.empty()){
                std::cout << "Received: " << reply << std::endl;
            }
        }
        std::cout << "\nWaiting ...\n" << std::endl;
    }
}

void repInterface(ZMQcoms *zmqcom){
    std::string request, reply;
    std::cout << "\nTo send a reply, wait for a request and then send reply...\n" << std::endl;
    while(true){
        sleep(2);
        std::cout << "\nWaiting for request message...\n" << std::endl;
        // std::getline(std::cin, request);
        request = (*zmqcom).replyReceive();
        if( !request.empty() ){
            std::cout << "Received: " << request << std::endl;
            std::cout << "\nEnter message to reply with..\n" << std::endl;
            std::getline(std::cin, reply);
            if( (*zmqcom).replySend(reply) ){
                std::cout << "Sent: " << reply << std::endl;
                reply.clear();
            }
        }
        
    }

}