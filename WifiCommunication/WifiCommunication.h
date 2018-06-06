#pragma once

#include "Arduino.h"
#include <ESP8266WiFi.h>
class WifiCommunication
{
public:
	WifiCommunication();
	~WifiCommunication();
	void SendCommand(char *command);
	
private:
	void ConnectToWifi();

	char* ssid = "Bia";
	char* password = "";

	const char* host = "192.168.0.108";
};

