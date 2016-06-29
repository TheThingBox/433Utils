/*
RF_Sniffer

Hacked from http://code.google.com/p/rc-switch/

by @justy to provide a handy RF code sniffer
*/

#include "RCSwitch.h"
#include "MqttWrapper.h"
#include "RFSniffer.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>

/*
* Usage : ./RFSniffer -b<MQTT Broker> -p<MQTT Port> -t<MQTT Topic>
*/
int main(int argc, char *argv[]) {

	//Error cases : could not setup wiringPi or bad arguments number
	if (::wiringPiSetup() == -1 || argc != 4)
	{
		return ERROR_RETURN;
	}

	//Get the arguments
	char *mqttBroker = 0;
	char *mqttTopic = 0;
	char *mqttPort = 0;
	for (int i = 0; i < argc; i++)
	{
		if (strlen(argv[i]) >= strlen(BROKER_SWITCH) && strncmp(argv[i], BROKER_SWITCH, strlen(BROKER_SWITCH)) == 0)
		{
			mqttBroker = argv[i]+2;
		}
		else if (strlen(argv[i]) >= strlen(PORT_SWITCH) && strncmp(argv[i], PORT_SWITCH, strlen(PORT_SWITCH)) == 0)
		{
			mqttPort = argv[i]+2;
		}
		else if (strlen(argv[i]) >= strlen(TOPIC_SWITCH) && strncmp(argv[i], TOPIC_SWITCH, strlen(TOPIC_SWITCH)) == 0)
		{
			mqttTopic = argv[i]+2;
		}
	}

	//Control arguments validity
	if(!mqttPort || !mqttBroker || !mqttTopic || strlen(mqttPort) == 0 || strlen(mqttBroker) == 0 || strlen(mqttTopic) == 0 || isdigit(mqttPort[0]) == 0)
	{
		return ERROR_RETURN;
	}

	RCSwitch mySwitch;
	MqttWrapper *mqtt;
	mqtt = new MqttWrapper(SNIFFER_MQTT_ID, mqttTopic, mqttBroker, atoi(mqttPort));
	mySwitch.enableReceive(PIN);

	while(1)
	{
		unsigned long currentCode = RCSwitch::popEvent();
		time_t now = time(0);
	
		char msg[20], codeStr[20];
		char *topic;

		topic = (char*)malloc(sizeof(char) * (strlen(mqttTopic) + 20));

		sprintf(codeStr, "%i", currentCode);
		strcpy(topic, mqttTopic);
		strcat(topic, "/");
		strcat(topic, codeStr);
		sprintf(msg, "%i", now);

		mqtt->send_message(msg, topic);
		free(topic);
	}
	return 0;
};