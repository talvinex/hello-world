#ifdef _WIN32
	#include <winsock2.h>
	#include <Ws2tcpip.h>

	//TODO: try defining close as closesocket
#else
	#include <sys/types.h>
	#include <sys/socket.h>

	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <unistd.h>
#endif

#include <iostream>
#include <string>



// Task: get Local-Network IP address of current device
std::string getLocalIp_4();

// TODO: StartWinSock() oder so, um WSAStartup() und alles aufzurufen