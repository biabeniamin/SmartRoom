#include "stdafx.h"
#include <string.h>
#include "TcpSocketServer.h"

using namespace std;

TcpSocketServer::TcpSocketServer(BYTE Port, void(*CommandReceivedFunction)(REMOTE_COMMAND, BYTE, BYTE))
{
	_commandReceivedFunction = CommandReceivedFunction;
	WSAStartup(MAKEWORD(2, 0), &WSAData);
	server = socket(AF_INET, SOCK_STREAM, 0);

	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(10000);

	bind(server, (SOCKADDR *)&serverAddr, sizeof(serverAddr));

}

void TcpSocketServer::Start()
{
	listen(server, 0);
	cout << "Listening for incoming connections..." << endl;

	char buffer[1024];
	int clientAddrSize = sizeof(clientAddr);
	while ((client = accept(server, (SOCKADDR *)&clientAddr, &clientAddrSize)) != INVALID_SOCKET)
	{
		int message;

		message = 0;

		memset(buffer,
			0,
			1024);
		cout << "Client connected!" << endl;
		recv(client, buffer, sizeof(buffer), 0);
		cout << "Client says: " << buffer << endl;

		message = buffer[0];
		_commandReceivedFunction((REMOTE_COMMAND)message, buffer[1], buffer[2]);

		closesocket(client);
		cout << "Client disconnected." << endl;
	}
}


TcpSocketServer::~TcpSocketServer()
{
}
