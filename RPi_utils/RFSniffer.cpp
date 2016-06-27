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
#include <pthread.h>
#include <unistd.h>

unsigned long currentCode = DEFAULT_CODE;
time_t firstCodeReceivedTime, lastCodeReceivedTime;

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
	bool reading = false;
	pthread_t threadedListenerThread;

	mqtt = new MqttWrapper(SNIFFER_MQTT_ID, mqttTopic, mqttBroker, atoi(mqttPort));
	mySwitch.enableReceive(PIN);

	while(1)
	{
		if(!reading)
		{
			currentCode = RCSwitch::popEvent();
			firstCodeReceivedTime = lastCodeReceivedTime = time(0);
			if(pthread_create(&threadedListenerThread, NULL, threadedListener, NULL) == 0)
			{
				reading = true;
			}
		}
		else
		{
			sleep(THRESHOLD);
			time_t now = time(0);
			if(now - lastCodeReceivedTime > THRESHOLD)
			{
				RCSwitch::pushEvent(0);
				
				reading = false;
			
				char msg[20], codeStr[20];
				char *topic;

				topic = (char*)malloc(sizeof(char) * (strlen(mqttTopic) + 20));

				sprintf(codeStr, "%i", currentCode);
				strcpy(topic, mqttTopic);
				strcat(topic, "/");
				strcat(topic, codeStr);
				sprintf(msg, "%i", lastCodeReceivedTime - firstCodeReceivedTime);

				mqtt->send_message(msg, topic);
				free(topic);
				sleep(1);
			}
		}
	}
	return 0;
};

void *threadedListener(void *unused){
	unsigned long code = 1;
	while(code != 0){
		code = RCSwitch::popEvent();
		if(code == currentCode)
		{
			lastCodeReceivedTime = time(0);
		}
	}
};