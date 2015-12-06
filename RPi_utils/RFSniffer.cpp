/*
  RF_Sniffer

  Hacked from http://code.google.com/p/rc-switch/

  by @justy to provide a handy RF code sniffer
*/

#include "RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>

// This pin is not the first pin on the RPi GPIO header!
// Consult https://projects.drogon.net/raspberry-pi/wiringpi/pins/
// for more information.
const int PIN = 2;

int main(int argc, char *argv[]) {

    if (::wiringPiSetup() == -1) return 1;

    RCSwitch mySwitch;
    mySwitch.enableReceive(PIN);  // Receiver on inerrupt 0 => that is pin #2
    ::printf("%i", RCSwitch::popEvent());

    return 0;
}
