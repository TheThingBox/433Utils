#ifndef _MQTTWRAPPERH_
#define _MQTTWRAPPERH_

#include <mosquittopp.h>
#include <stdio.h>
#include <string.h>

class MqttWrapper : public mosqpp::mosquittopp
{
private:
	const char* host;
	const char* id;
	const char* topic;
	int port;
	int keepalive;
	bool status;

	void on_connect(int rc);
	void on_disconnect(int rc);
public:
	MqttWrapper(const char *id, const char * _topic, const char *host, int port);
	~MqttWrapper();
	bool send_message(const char * _message, const char * _topic);
	bool is_alive();
};

#endif //_MQTTWRAPPERH_