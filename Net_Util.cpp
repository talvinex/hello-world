#include "Net_Util.hpp"



// getLocalIp
//
// Task: get Local-Network IP address of current device. 
//       WSaStartUp has to be called before using this function on Windows.
//
std::string getLocalIp_4(){


	std::string return_string;

	// Temporary socket and dest. address for the request

	#ifdef _WIN32
		SOCKET temp_socket = socket(AF_INET, SOCK_DGRAM, 0);
		if (temp_socket == INVALID_SOCKET){
			std::cout << "socket() int getLocalIp() failed: " << WSAGetLastError() << std::endl;
			exit(1);
		}
	#else
		int temp_socket = socket(AF_INET, SOCK_DGRAM, 0);
		if (temp_socket == -1){
			perror("socket() in getLocalIp() failed");
			_exit(1);
		}
	#endif
	
	sockaddr_in temp_addr;
	temp_addr.sin_family = AF_INET;
	temp_addr.sin_port = htons(53);
	temp_addr.sin_addr.s_addr = inet_addr("8.8.8.8");


	// Establish connection to google dns server

	if (connect(temp_socket, (struct sockaddr*) &temp_addr, sizeof(temp_addr)) == -1){
		#ifdef _WIN32
			std::cout << "connect() in getLocalIp() failed: " << WSAGetLastError() << std::endl;
			exit(1);
		#else
			perror("connect() in getLocalIp() failed");
			_exit(1);
		#endif
		
	}

	// Get local address by getsockname()

	sockaddr_in local_addr;
    socklen_t local_addr_len = sizeof(local_addr);

    if(getsockname(temp_socket, (sockaddr*) &local_addr, &local_addr_len) == -1){
    	#ifdef _WIN32
			std::cout << "getsockname() in getLocalIp() failed: " << WSAGetLastError() << std::endl;
			exit(1);
		#else
			perror("getsockname() in getLocalIp() failed");
			_exit(1);
		#endif
    }

    // Turn byte address into readable address and save at return string

	char* p = inet_ntoa(local_addr.sin_addr);
	return_string = p;	// fill return string with return value from inet_ntoa

    if (p == NULL){
    	#ifdef _WIN32
			std::cout << "inet_ntop() in getLocalIp() failed: " << WSAGetLastError() << std::endl;
			exit(1);
		#else
			perror("inet_ntop() in getLocalIp() failed");
			_exit(1);
		#endif
    }

    // Close Socket and return local ip address as string

    #ifdef _WIN32
		closesocket(temp_socket);
	#else
		close(temp_socket);
	#endif


    return (return_string);

} // getLocalIp