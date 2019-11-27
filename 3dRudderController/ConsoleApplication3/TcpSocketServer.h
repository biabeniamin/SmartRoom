#pragma once
#include <Windows.h>
#include <iostream>

enum REMOTE_COMMAND_ENUM
{
	MEDIA_PLAY = 0,
	MEDIA_PAUSE = 1,
	MEDIA_PLAYPAUSE = 2,
	MEDIA_NEXT = 3,
	MEDIA_PREVIOUS = 4,
	MEDIA_VOLUMEUP = 5,
	MEDIA_VOLUMEDOWN = 6,
	MEDIA_VOLUMEMUTE = 7,
	SPACEBAR = 8,
	LOCK = 9,
	STOP_MUSIC = 10,
	SWITCH_AC = 11,
	ALERT = 12
};

typedef enum REMOTE_COMMAND_ENUM REMOTE_COMMAND;

class TcpSocketServer
{
public:
	TcpSocketServer(BYTE, void(*)(REMOTE_COMMAND, BYTE, BYTE));
	~TcpSocketServer();

	void Start();
private:
	WSADATA WSAData;

	SOCKET server, client;

	SOCKADDR_IN serverAddr, clientAddr;

	void(*_commandReceivedFunction)(REMOTE_COMMAND, BYTE, BYTE);
};

