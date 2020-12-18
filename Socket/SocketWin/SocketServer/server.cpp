// FileName: server.cpp
// Description: Create server socket application
// Author: Jiejing.Ma
// Update: 2020/12/11

#undef UNICODE

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <WinSock2.h>  // socket
#include <WS2tcpip.h>  // TCP/IP
#include <iphlpapi.h>  // ip helper APIs
#include <stdlib.h>
#include <iostream>

// Link to Ws2_32.lib
#pragma comment(lib,"Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

using namespace std;

int main()
{
	WSADATA wasData;
	int iRes;

	// Create a SOCKET object to listen for client connections
	SOCKET ListenSocket = INVALID_SOCKET;
	// Create temporary ClientSocket for accepting connetions from clients
	SOCKET ClientSocket = INVALID_SOCKET;

	addrinfo* result = NULL;
	addrinfo hints;

	char recvbuf[DEFAULT_BUFLEN];
	int iSendRes;
	int recvbuflen = DEFAULT_BUFLEN;

	// 1. Initialize WinSock
	iRes = WSAStartup(MAKEWORD(2, 2), &wasData);
	if (iRes != 0)
	{
		cout << "WSAStartup failed: " << iRes << endl;
		return 1;
	}

	// 2. Create server socket
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	iRes = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iRes != 0)
	{
		cout << "getaddrinfo failed: " << iRes << endl;
		WSACleanup();
		return 1;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	// Check for errors to ensure that the socket is valid socket
	if (ListenSocket == INVALID_SOCKET)
	{
		cout << "Error at socket():"<< WSAGetLastError() << endl;
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// 3. Bind socket
	// Setup the TCP listening socket
	iRes = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iRes == SOCKET_ERROR)
	{
		cout << "bind failed with error: " << WSAGetLastError() << endl;
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	// free memory allocated by getaddrinfo() for address information
	freeaddrinfo(result);

	// 4. Listening on a Socket
	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		cout << "Listen failed with error: " << WSAGetLastError() << endl;
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// 5. Accepting a Connetion
	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET)
	{
		cout << "accept failed: " << WSAGetLastError() << endl;
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// No longer need server socket
	closesocket(ListenSocket);

	// 6. Receiving and Sending Data on the Server
	iRes = 1;
	// Receive until the peer shuts down the connection
	while (iRes > 0)
	{
		iRes = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iRes > 0)
		{
			cout << "Server Bytes received: " << recvbuf << endl;

			// Echo the buffr back to the sender
			iSendRes = send(ClientSocket, recvbuf, iRes, 0);
			if (iSendRes == SOCKET_ERROR)
			{
				cout << "send failed: " << WSAGetLastError() << endl;
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}
			cout << "Server Bytes sent: " << iSendRes << endl;
		}
		else
		{
			cout <<"recv failed: " << WSAGetLastError() << endl;
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}
	}

	// 7.Disconneting the Server
	// shutdown the send half of the connetiong since no more data will be sent
	iRes = shutdown(ClientSocket, SD_SEND);
	if (iRes == SOCKET_ERROR)
	{
		cout << "shutdown failed: " << WSAGetLastError() << endl;
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	// cleanup
	closesocket(ClientSocket);
	WSACleanup();

	return 0;

}
