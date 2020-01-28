#ifndef ZMQCOMS_HPP_
#define ZMQCOMS_HPP_

#include <zmq.hpp>
#include <string>
#include <sstream>
#include "zhelpers.hpp"

class ZMQcoms{
    private:
        zmq::context_t* _context;
        zmq::socket_t* _socket;

    public:
        enum Pattern{
            PUB = 1,
            SUB = 2,
            REQ = 3,
            REP = 4
        };
        ZMQcoms();

        void setup(Pattern type, int port);
        void subscribeToTopic(std::string topic);

        void syncPub(int port);
        void syncSub(int port);
        
        void publishMessage(std::string topic, std::string message);
        std::string subscribeMessage();

        std::string requestMessage(std::string message);
        ~ZMQcoms();
};

#endif