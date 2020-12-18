// FileName: client.cpp
// Description: Create client socket application
// Author: Jiejing.Ma
// Update: 2020/12/11

#undef UNICODE

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iphlpapi.h>
#include <iostream>

// Need to link with Ws2_32.li, Mswsock.lib, Advapi32.lib
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"Mswsock.lib")
#pragma comment(lib,"Advapi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

using namespace std;

int main(int argc,char ** argv)
{
	WSADATA wsaData;
	int iRes;

	SOCKET ConnectSocket  = INVALID_SOCKET;

	addrinfo hints;
	addrinfo* result = NULL, *ptr=NULL;

	const char *sendbuf="hello";
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	// 1. Initialize WinSock
	iRes = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iRes != 0)
	{
		cout << "WSAStartup failed: " << iRes << endl;
		return 1;
	}

	// 2. Create socket
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_socktype = SOCK_STREAM;

	// Resolve the server address and port
	iRes = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
	if (iRes != 0)
	{
		cout << "getaddrinfo failed:" << iRes << endl;
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{
		// Create a SOCKET for connecting to server	
		ConnectSocket  = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (ConnectSocket  == INVALID_SOCKET)
		{
			cout << "Error at socket():" << WSAGetLastError() << endl;
			WSACleanup();
			return 1;
		}

		// 3.Connect to Server
		iRes = connect(ConnectSocket , ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iRes == SOCKET_ERROR)
		{
			closesocket(ConnectSocket );
			ConnectSocket  = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket  == INVALID_SOCKET)
	{
		cout << "Unable to connect to server!" << endl;
		WSACleanup();
		return 1;
	}
	// 4. Send and Receive data
	// Send an initial buffer
	iRes = send(ConnectSocket , sendbuf, (int)strlen(sendbuf), 0);
	if (iRes == SOCKET_ERROR)
	{
		cout << "send faild: " << WSAGetLastError() << endl;
		closesocket(ConnectSocket );
		WSACleanup();
		return 1;
	}
	cout << "Client Bytes sent: " << iRes << endl;

	// shutdown the connection for sending since no more data will be sent
	// the client can still use the ConnectSocket for receiving data
	iRes = shutdown(ConnectSocket , SD_SEND);
	if (iRes == SOCKET_ERROR)
	{
		cout << "shutdown failed: " << WSAGetLastError() << endl;
		closesocket(ConnectSocket );
		WSACleanup();
		return 1;
	}

	iRes = 1;
	while (iRes>0)
	{
		iRes = recv(ConnectSocket , recvbuf, recvbuflen, 0);
		if (iRes > 0)
			printf("Client Bytes received: %d\n", iRes);
		else if (iRes == 0)
			printf("Connection closed\n");
		else
			printf("recv failed: %d\n", WSAGetLastError());
	}

	// 5. Disconnect
	iRes = shutdown(ConnectSocket , SD_SEND);
	if (iRes == SOCKET_ERROR)
	{
		cout << "shutdown failed: " << WSAGetLastError() << endl;
		closesocket(ConnectSocket );
		WSACleanup();
		return 1;
	}

	// cleanup
	closesocket(ConnectSocket );
	WSACleanup();

	return 0;
}