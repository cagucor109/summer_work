#include "ZMQcoms.hpp"

ZMQcoms::ZMQcoms(){

}

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

void ZMQcoms::subscribeToTopic(std::string topic){
    const char *filter = topic.c_str();
    (*_socket).setsockopt(ZMQ_SUBSCRIBE, filter, strlen (filter));
}

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
        
void ZMQcoms::publishMessage(std::string topic, std::string message){
    s_sendmore (*_socket, topic);
    s_send (*_socket, message);
}

std::string ZMQcoms::subscribeMessage(){
    std::string topic = s_recv (*_socket);
	std::string data = s_recv (*_socket);
    return data;
}

std::string ZMQcoms::requestMessage(std::string message){
    s_send(*_socket, message);
    return s_recv(*_socket);
}

ZMQcoms::~ZMQcoms(){
    delete _context;
    delete _socket;
}