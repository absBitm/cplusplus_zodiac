
#ifndef ____ipcPubService__
#define ____ipcPubService__

#include "../util/helpers/zmq/zmq.hpp"
//#include "service.h"
#include <iostream>
#include <string> 
#include <sstream>

using namespace std;

#define PRE_ADD "tcp://*:"

class IpcPubService {
	
	private:
		zmq::socket_t *socket;	
		zmq::context_t *context;

	public:
		IpcPubService(int port, string host);
		~IpcPubService();	
		void publish(string channel, string message);
};

IpcPubService::IpcPubService(int port , string host){
	stringstream ss;
	ss << PRE_ADD << port;
	string address = ss.str(); 
	this->context  = new zmq::context_t;
	this->socket   = new zmq::socket_t(*context, ZMQ_PUB) ;
	this->socket->bind(address.c_str());
	
}

void IpcPubService::publish(string channel,string message) {
	//cout<<" publishing channel : "<<channel<<"\n message : "<<message<<endl<<flush;

	zmq::message_t channelPrefix(channel.size());
	// snprintf ((char *) channelPrefix.data(), channel.size()+1 ,channel.c_str());
	memcpy (channelPrefix.data(), channel.data(), channel.size());
	socket->send(channelPrefix,ZMQ_SNDMORE);

	zmq::message_t payLoad(message.size());
	memcpy (payLoad.data(), message.data(), message.size());
	socket->send(payLoad);
	
}

IpcPubService::~IpcPubService() {
	this->socket->~socket_t();
	this->context->~context_t();
}

#endif
