#include "WifiCommunication.h"
//#define DO_DEBUG



WifiCommunication::WifiCommunication()
{
	ConnectToWifi();
}

WifiCommunication::~WifiCommunication()
{
}

void WifiCommunication::ConnectToWifi()
{
	Serial.println();
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(ssid);

	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
}

void WifiCommunication::SendCommand(char *command)
{

	if (WiFi.status() != WL_CONNECTED)
	{
		ConnectToWifi();
		delay(200);
	}
	
	Serial.print("connecting to ");
	Serial.println(host);

	// Use WiFiClient class to create TCP connections
	WiFiClient client;
	const int httpPort = 80;
	
	if (!client.connect(host, httpPort)) {
		Serial.println("connection failed");
		return;
	}

	String url = "/wifiLan.php?motionDetectedStairs";

	Serial.print("Requesting URL: ");
	Serial.println(url);

	// This will send the request to the server
	client.print(String("GET ") + url + " HTTP/1.1\r\n" +
		"Host: " + host + "\r\n" +
		"Connection: close\r\n\r\n");
	unsigned long timeout = millis();
	while (client.available() == 0) {
		if (millis() - timeout > 5000) {
			Serial.println(">>> Client Timeout !");
			client.stop();
			return;
		}
	}

	// Read all the lines of the reply from server and print them to Serial
	while (client.available()) {
		String line = client.readStringUntil('\r');
		Serial.print(line);
	}

	Serial.println();
	Serial.println("closing connection");
}