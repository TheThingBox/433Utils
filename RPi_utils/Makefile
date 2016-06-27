all: send codesend RFSniffer RFSnifferLoop

send: RCSwitch.o send.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -pthread $+ -o $@ -lwiringPi
	
codesend: RCSwitch.o codesend.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -pthread $+ -o $@ -lwiringPi
	
RFSniffer: RCSwitch.o RFSniffer.o MqttWrapper.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -pthread $+ -o $@ -lwiringPi -lmosquittopp
	
RFSnifferLoop: RCSwitch.o RFSnifferLoop.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -pthread $+ -o $@ -lwiringPi

clean:
	$(RM) *.o send codesend RFSniffer RFSnifferLoop
