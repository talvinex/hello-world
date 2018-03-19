#include <iostream>

#include "Net_Util.hpp"

#include <string>

#define BUF_SIZE 4096
#define SERVER_PORT 9002
#define SERVER_ADDRESS "46.82.156.197"


using std::cout;
using std::cin;
using std::endl;

int main(){
	
	// Get Server IP Address
	std::string addr_input;
	cout << "IP-Adresse des Servers eingeben: ";
	std::getline(cin, addr_input);

	// Setting up socket (/and Winsock)
	#ifdef _WIN32
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0){
			cout << "WSAStartup() failed: " << WSAGetLastError() << endl;
			return 1;
		}

		SOCKET network_socket;
		network_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (network_socket == INVALID_SOCKET){
			cout << "socket() failed: " << WSAGetLastError() << endl;
			return 1;
		}
	#else
		int network_socket;
		network_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (network_socket == -1){
			perror("socket() failed");
			return 1;
		}
	#endif

	// Get local IP adress

	std::string c_LocalIp;
	c_LocalIp = getLocalIp_4();

	cout << "Local IP address: " << c_LocalIp << endl;

	// Define server address
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons (SERVER_PORT);
	server_addr.sin_addr.s_addr = inet_addr(addr_input.c_str());

	// Connect to server
	if (connect(network_socket, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1){
		
		#ifdef _WIN32
			cout << "connect() failed: " << WSAGetLastError() << endl;
		#else
			perror("connect() failed");
		#endif

		return 1;
	}


	// Receive data from server

	char recv_buffer[BUF_SIZE];
	int recv_bytes;

	if ((recv_bytes = recv(network_socket, recv_buffer, sizeof(recv_buffer), 0)) < 0){
		
		#ifdef _WIN32
			cout << "recv() failed: " << WSAGetLastError() << endl;
		#else
			perror("recv() failed");
		#endif

		return 1;
	}

	// Print server welcome msg

	recv_buffer[recv_bytes] = '\0';
	cout << recv_buffer << endl;

	cout << "\n- - - - - CHAT OPENED - - - - - \nType message or \"/exit\" to leave chatroom.\n" << endl;
	

	// - - - MAIN LOOP - - - //
	//						 //

	while(true){

		// Send message to server

		std::string msg_buffer;
		cout << "You: ";
		std::getline(cin, msg_buffer);

		if (send(network_socket, msg_buffer.c_str(), strlen(msg_buffer.c_str()), 0) == -1){
			#ifdef _WIN32
				cout << "send() failed: " << WSAGetLastError() << endl;
			#else
				perror("send() failed");
			#endif

			return 1;
		}

		// Test for exit command
		if (msg_buffer == "/exit"){
			break;
		}


		// Receive data from server

		memset(recv_buffer, 0, sizeof(recv_buffer));
		recv_bytes = -1;

		cout << "Server: " << std::flush;
		if ((recv_bytes = recv(network_socket, recv_buffer, sizeof(recv_buffer), 0)) < 0){
			
			#ifdef _WIN32
				cout << "recv() failed: " << WSAGetLastError() << endl;
			#else
				perror("recv() failed");
			#endif

			return 1;
		}

		// Terminate buffer
		recv_buffer[recv_bytes] = '\0'; 

		// test for exit command
		if (strcmp(recv_buffer, "/exit") == 0){
			break;
		}

		// Print server response
		cout << recv_buffer << endl;

	} // while (true)


	cout << "\n- - - - - CHAT CLOSED - - - - - \n" << endl;
	
	

	// Close sockets

	#ifdef _WIN32
		closesocket(network_socket);
	#else
		close(network_socket);
	#endif
	
	#ifdef _WIN32
	system("PAUSE");
	#endif
	
	return 0;
}