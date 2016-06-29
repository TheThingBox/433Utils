#ifndef _RFSNIFFERH_
#define _RFSNIFFERH_

#define SNIFFER_MQTT_ID	"RFSniffer"
#define BROKER_SWITCH	"-b"
#define PORT_SWITCH		"-p"
#define TOPIC_SWITCH	"-t"
#define ERROR_RETURN	1

// This pin is not the first pin on the RPi GPIO header!
// Consult https://projects.drogon.net/raspberry-pi/wiringpi/pins/
// for more information.
const int PIN = 22;

#endif //_RFSNIFFERH_