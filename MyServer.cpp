#include <iostream>

#include "Net_Util.hpp"


#define BUF_SIZE 4096
#define SERVER_PORT 9002
#define SERVER_ADDRESS "192.168.178.25"


using std::cout;
using std::cin;
using std::endl;

int main(int argc, char* argv[]){


	// Setting up socket (/and Winsock)

	#ifdef _WIN32
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0){
			cout << "WSAStartup() failed: " << WSAGetLastError() << endl;
			return 1;
		}

		SOCKET server_socket;
		server_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (server_socket == INVALID_SOCKET){
			cout << "socket() failed: " << WSAGetLastError() << endl;
			return 1;
		}
	#else
		int server_socket;
		server_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (server_socket == -1){
			perror("socket() failed");
			return 1;
		}
	#endif


	// Get local IP adress

	std::string c_LocalIp;
	c_LocalIp  = getLocalIp_4();

	cout << "Local IP address: " << c_LocalIp << endl;


	// Define server address

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons (SERVER_PORT);
	server_addr.sin_addr.s_addr = inet_addr(c_LocalIp.c_str());


	// Bind socket to specified IP and port

	if (bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1){

		#ifdef _WIN32
			cout << "bind() failed: " << WSAGetLastError() << endl;
		#else
			perror("bind() failed");
		#endif
		
		return 1;
	}


	// Start listening

	if (listen(server_socket, 3) == -1){

		#ifdef _WIN32
			cout << "listen() failed: " << WSAGetLastError() << endl;
		#else
			perror("listen() failed");
		#endif
		
		return 1;
	}


	// Client holen

	struct sockaddr_in client_addr;
	socklen_t client_addr_size;
	client_addr_size = sizeof(client_addr);

	#ifdef _WIN32
		SOCKET client_socket;
		if ((client_socket = accept(server_socket, (struct sockaddr*) &client_addr, &client_addr_size)) == INVALID_SOCKET){
			cout << "accept() failed: " << WSAGetLastError() << endl;
			return 1;
		}
	#else
		int client_socket;
		if ((client_socket = accept(server_socket, (struct sockaddr*) &client_addr, &client_addr_size)) == -1){
			perror("accept() failed");
			return 1;
		}
	#endif
	
	cout << "Client connected with IP --> " << inet_ntoa(client_addr.sin_addr) << endl;

	cout << "\n- - - - - CHAT OPENED - - - - - \n" << endl;


	// Send welcome message to client

	char server_welcome_msg[] = "Welcome, you have reached the server.";

	if (send(client_socket, server_welcome_msg, sizeof(server_welcome_msg), 0) == -1){

		#ifdef _WIN32
			cout << "send() failed: " << WSAGetLastError() << endl;
		#else
			perror("send() failed");
		#endif

		return 1;
	}


	// - - - MAIN LOOP - - - //
	//						 //

	while(true){

		// Wait for message from client

		char recv_buffer[BUF_SIZE];
		int recv_bytes;

		cout << "Client: " << std::flush	;
		if ((recv_bytes = recv(client_socket, recv_buffer, sizeof(recv_buffer), 0)) < 0){
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


		// Send one more custom message to client 

		std::string msg_buffer;
		cout << "You: ";
		std::getline(cin, msg_buffer);

		if (send(client_socket, msg_buffer.c_str(), strlen(msg_buffer.c_str()), 0) == -1){

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

	} // while(true)


	cout << "\n\n- - - - - CHAT CLOSED - - - - - \n" << endl;

	// Close sockets

	#ifdef _WIN32
		closesocket(client_socket);
		closesocket(server_socket);

		WSACleanup();
	#else
		close(client_socket);
		close(server_socket);
	#endif
	

	#ifdef _WIN32
		system("PAUSE");
	#endif

	
	return 0;
}