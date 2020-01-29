#include "ZMQcoms.hpp"

ZMQcoms::ZMQcoms(){

}

// Setup the socket to the pattern specified
void ZMQcoms::setup(Pattern type, int port){
    _context = new zmq::context_t (1);
    std::stringstream ss;
    std::string IP = (type == REQ || type == SUB) ? "localhost":"*";
    if(type == REQ){
        ss << "tcp://localhost:" << port;
        _socketReq = new zmq::socket_t(*_context, type);
        (*_socketReq).connect(ss.str());
    }else if(type == SUB){
        ss << "tcp://localhost:" << port;
        _socketSub = new zmq::socket_t(*_context, type);
        (*_socketSub).connect(ss.str());
    }else if(type == REP){
        ss << "tcp://*:" << port;
        _socketRep = new zmq::socket_t(*_context, type);
        (*_socketRep).bind(ss.str());
    }else{
        ss << "tcp://*:" << port;
        _socketPub = new zmq::socket_t(*_context, type);
        (*_socketPub).bind(ss.str());
    }
}

// Setup a subscription to a topic for a SUB socket
void ZMQcoms::subscribeToTopic(std::string topic){
    const char *filter = topic.c_str();
    (*_socketSub).setsockopt(ZMQ_SUBSCRIBE, filter, strlen (filter));
}

// Remove a subscription to a topic for a SUB socket
void ZMQcoms::unsubscribeFromTopic(std::string topic){
    const char *filter = topic.c_str();
    (*_socketSub).setsockopt(ZMQ_UNSUBSCRIBE, filter, strlen (filter));
}

// not sure if necessary
void ZMQcoms::syncPub(int port){
    zmq::socket_t syncservice (*_context, ZMQ_REP);
    std::stringstream ss;
    ss << "tcp://*:" << port;
    syncservice.bind(ss.str());
        
	//  - wait for synchronization request
	s_recv (syncservice);
       
	//  - send synchronization reply
	s_send (syncservice, "");

}

// not sure if necessary
void ZMQcoms::syncSub(int port){
    zmq::socket_t syncclient (*_context, ZMQ_REQ);
    std::stringstream ss;
    ss << "tcp://localhost:" << port;
    syncclient.connect(ss.str());

    //  - send a synchronization request
    s_send (syncclient, "");

    //  - wait for synchronization reply
    s_recv (syncclient);

}

// publish a message to the specified topic
void ZMQcoms::publishMessage(std::string topic, std::string message){
    s_sendmore (*_socketPub, topic);
    s_send_nowait (*_socketPub, message);
}

// receive the message from a topic
std::vector<std::string> ZMQcoms::subscribeMessage(){
    std::vector<std::string> message;
    std::string topic = s_recv_nowait (*_socketSub);
    message.push_back(topic);
	std::string data = s_recv_nowait (*_socketSub);
    message.push_back(data);
    
    return message;
}

bool ZMQcoms::requestSend(std::string message){
    return s_send_nowait(*_socketReq, message);
}

std::string ZMQcoms::requestReceive(){
    return s_recv(*_socketReq);
}

std::string ZMQcoms::replyReceive(){
    return s_recv_nowait(*_socketRep);
}

bool ZMQcoms::replySend(std::string message){
    return s_send(*_socketRep, message);
}

ZMQcoms::~ZMQcoms(){
    delete _context;
    delete _socketPub;
    delete _socketSub;
    delete _socketReq;
    delete _socketRep;
}