#include "MqttWrapper.h"

MqttWrapper::MqttWrapper(const char * _id,const char * _topic, const char * _host, int _port) : mosquittopp(_id)
{
	mosqpp::lib_init();// Mandatory initialization for mosquitto library
	this->keepalive = 60;// Basic configuration setup for myMosq class
	this->status = false;
	this->id = _id;
	this->port = _port;
	this->host = _host;
	this->topic = _topic;
	connect(host, port, keepalive);// non blocking connection to broker request
	loop_start();// Start thread managing connection / publish / subscribe
};

MqttWrapper::~MqttWrapper() 
{
	loop_stop();
	mosqpp::lib_cleanup();
};

bool MqttWrapper::send_message(const  char * _message, const char * _topic)
{
	int ret = publish(NULL,(_topic != NULL)?_topic:this->topic,strlen(_message),_message,1,false);
	return ( ret == MOSQ_ERR_SUCCESS );
};

void MqttWrapper::on_disconnect(int rc) {
	this->status = false;
};

void MqttWrapper::on_connect(int rc)
{
	this->status = true;
};

bool MqttWrapper::is_alive(){
	return this->status;
};