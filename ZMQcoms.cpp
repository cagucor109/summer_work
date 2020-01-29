#include "ZMQcoms.hpp"

ZMQcoms::ZMQcoms(){

}

// Setup the socket to the pattern specified
void ZMQcoms::setup(Pattern type, int port){
    _context = new zmq::context_t (1);
    _socket = new zmq::socket_t(*_context, type);
    std::stringstream ss;
    std::string IP = (type == REQ || type == SUB) ? "localhost":"*";
    if(type == REQ || type == SUB){
        ss << "tcp://localhost:" << port;
        (*_socket).connect(ss.str());
    } else{
        ss << "tcp://*:" << port;
        (*_socket).bind(ss.str());
    }
}

// Setup a subscription to a topic for a SUB socket
void ZMQcoms::subscribeToTopic(std::string topic){
    const char *filter = topic.c_str();
    (*_socket).setsockopt(ZMQ_SUBSCRIBE, filter, strlen (filter));
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
    s_sendmore (*_socket, topic);
    s_send_nowait (*_socket, message);
}

// receive the message from a topic
std::string ZMQcoms::subscribeMessage(int option){
    std::string topic = s_recv_nowait (*_socket);
	std::string data = s_recv_nowait (*_socket);
    if(option == 0){
        return topic;
    }else if(option == 1){
        return data; // this is the default
    }else{
        return ""; // add error here
    }
    
}

bool ZMQcoms::requestSend(std::string message){
    return s_send_nowait(*_socket, message);
}

std::string ZMQcoms::requestReceive(){
    return s_recv(*_socket);
}

std::string ZMQcoms::replyReceive(){
    return s_recv_nowait(*_socket);
}

bool ZMQcoms::replySend(std::string message){
    return s_send(*_socket, message);
}

ZMQcoms::~ZMQcoms(){
    delete _context;
    delete _socket;
}