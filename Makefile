#this is a Makefile

all: both
	#g++ MyServer.cpp Net_Util.cpp -o MyServer

both: MyServer.o MyClient.o Net_Util.o
	g++ MyServer.o Net_Util.o -o MyServer
	g++ MyClient.o Net_Util.o -o MyClient

MyServer.o: MyServer.cpp
	g++ -c MyServer.cpp

Net_Util.o: Net_Util.cpp
	g++ -c Net_Util.cpp

clean: 
	rm -rf *o MyServer MyClient 