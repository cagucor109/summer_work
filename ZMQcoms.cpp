#include "ZMQcoms.hpp"

ZMQcoms::ZMQcoms(){
    _context = new zmq::context_t (1);
}

// Setup the socket to the pattern specified
void ZMQcoms::setUp(Pattern type){
    if(type == REQ){
        _socketReq = new zmq::socket_t(*_context, type);
    }else if(type == SUB){
        _socketSub = new zmq::socket_t(*_context, type);
    }else if(type == REP){
        _socketRep = new zmq::socket_t(*_context, type);
    }else{
        _socketPub = new zmq::socket_t(*_context, type);
    }
}

// Connects or binds a particular socket to a port
void ZMQcoms::setConnection(Pattern type, int port, std::string bind_con){
    std::stringstream ss;
    std::string IP = (bind_con == "connect") ? "localhost":"*";
    ss << "tcp://" << IP << ":" << port;
    if(type == REQ){
        if(bind_con == "connect"){
            (*_socketReq).connect(ss.str());
        }else{
            (*_socketReq).bind(ss.str());
        }
    }else if(type == SUB){
        if(bind_con == "connect"){
            (*_socketSub).connect(ss.str());
        }else{
            (*_socketSub).bind(ss.str());
        }
    }else if(type == REP){
        if(bind_con == "connect"){
            (*_socketRep).connect(ss.str());
        }else{
            (*_socketRep).bind(ss.str());
        }
    }else{
        if(bind_con == "connect"){
            (*_socketPub).connect(ss.str());
        }else{
            (*_socketPub).bind(ss.str());
        }
    }
}

void ZMQcoms::setUpPrompt(){
    std::cout << "ZMQ set up prompt initated..." << std::endl;
    std::cout << "What type of sockets would you like on the zmq interface?" << std::endl;
    std::cout << "(Only one of each type is permitted)\n" << std::endl;
    std::cout << "Publisher:\tPUB\nSubscriber:\tSUB\nRequester:\tREQ\nReplier:\tREP" << std::endl;
    
    std::string socketType;
    std::string more;
    ZMQcoms::Pattern type;
    std::vector<std::string> sockets;

    while(true){
        printf("\nDesired socket type:\t");
        std::cin >> socketType;

        if( socketType == "PUB" || socketType == "Publisher" ){
            type = ZMQcoms::Pattern::PUB;
            sockets.push_back("Publisher");
        }else if( socketType == "SUB" || socketType == "Subscriber" ){
            type = ZMQcoms::Pattern::SUB;
            sockets.push_back("Subscriber");
        }else if( socketType == "REQ" || socketType == "Requester" ){
            type = ZMQcoms::Pattern::REQ;
            sockets.push_back("Requester");
        }else if( socketType == "REP" || socketType == "Replier" ){
            type = ZMQcoms::Pattern::REP;
            sockets.push_back("Replier");
        }else{
            std::cout << "\nIncorrect input, please try one of the options specified above..." << std::endl;
            continue;
        }

        this->setUp(type);
        std::cout << socketType << " correctly initated!" << std::endl;

        std::cout << "\nWould you like another type of socket?" << std::endl;
        printf("yes(y) or no(n)\t");
        std::cin >> more;
        if (more == "no" || more == "n"){
            break;
        }else if ( more == "yes" || more == "y"){
            
        }else{
            std::cout << "Input not recognised, assumed as yes.." << std::endl;
        }
    }
    std::cout << "\nSocket type input finished!" << std::endl;
    std::cout << "\nPlease enter the desired port(s) followed by connect or bind for each socket type chosen...\n" << std::endl;

    int port;
    std::string bind_con;

    for(int i = 0; i < sockets.size(); i++){
        while(true){
            std::cout << sockets.at(i) << ": " << std::endl;
            printf("Port number:\t\t");
            std::cin >> port;
            printf("Connect or bind:\t");
            std::cin >> bind_con;

            if( sockets.at(i) == "Publisher" ){
                type = ZMQcoms::Pattern::PUB;
            }else if( sockets.at(i) == "Subscriber" ){
                type = ZMQcoms::Pattern::SUB;
                std::string topic;
                std::string answer;
                std::cout << "\nPlease enter the topics you want to subscribe to..." << std::endl;
                std::cout << "Input space to subscribe to all topics...\n" << std::endl;
                while(true){
                    printf("Topic:\t");
                    std::cin >> topic;
                    std::cin.ignore();
                    subscribeToTopic(topic);
                    std::cout << "\nWould you like to subscribe to another topic?" << std::endl;
                    printf("yes(y) or no(n):\t");
                    std::cin >> answer;
                    if(answer == "yes" || answer == "y"){
                        continue;
                    }else if(answer == "no" || answer == "n"){
                        break;
                    }else{
                        std::cout << "Input not recognised, assumed as yes..." << std::endl;
                    }
                }
                std::cout << "\nSubscription of topics finished!\n" << std::endl;
            }else if( sockets.at(i) == "Requester" ){
                type = ZMQcoms::Pattern::REQ;
            }else{
                type = ZMQcoms::Pattern::REP;
            }

            this->setConnection(type, port, bind_con);
            std::cout << "\nConnection set successfully!\n" << std::endl;
            std::cout << "\nDo you want to connect to another port?" << std::endl;
            printf("yes(y) or no(n): ");
            std::string answer;
            std::cin >> answer;
            if(answer == "n" || answer == "no"){
                break;
            }else{
                continue;
            }
        }
        
    }

    std::cout << "Setup finished for the ZMQ interface!" << std::endl;
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
    // nested if statements to ensure correct order of topic and data
    if(!topic.empty()){
        message.push_back(topic);
        std::string data = s_recv_nowait (*_socketSub);
        if(!data.empty()){
            message.push_back(data);
        }
    }
    

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