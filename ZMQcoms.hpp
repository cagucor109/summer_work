#ifndef ZMQCOMS_HPP_
#define ZMQCOMS_HPP_

#include <zmq.hpp>
#include <string>
#include <sstream>
#include <vector>
#include "zhelpers.hpp"

class ZMQcoms{
    private:
        zmq::context_t* _context;
        // possibly make a vector of sockets to allow arbitrary amount
        zmq::socket_t* _socketPub;
        zmq::socket_t* _socketSub;
        zmq::socket_t* _socketReq;
        zmq::socket_t* _socketRep;

    public:
        enum Pattern{
            PUB = 1,
            SUB = 2,
            REQ = 3,
            REP = 4
        };
        ZMQcoms();

        //possibly include setUp prompt

        void setup(Pattern type, int port, std::string bind_con);
        void subscribeToTopic(std::string topic);
        void unsubscribeFromTopic(std::string topic);

        void syncPub(int port);
        void syncSub(int port);
        
        void publishMessage(std::string topic, std::string message);
        std::vector<std::string> subscribeMessage();

        bool requestSend(std::string message);
        std::string requestReceive();

        std::string replyReceive();
        bool replySend(std::string message);
        ~ZMQcoms();
};

#endif