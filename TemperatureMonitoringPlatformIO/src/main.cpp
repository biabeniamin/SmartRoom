/*
    This sketch sends data via HTTP GET requests to data.sparkfun.com service.

    You need to get streamId and privateKey at data.sparkfun.com and paste them
    below. Or just customize this script to talk to other HTTP servers.

*/

#include <ESP8266WiFi.h>



#include <SHT1x.h>

// Specify data and clock connections and instantiate SHT1x object
#define dataPin  0
#define clockPin 5
SHT1x sht1x(dataPin, clockPin);



const char* ssid     = "Bia";
const char* password = "";

const char* host = "http://iot.robofun.ro/api/v1/senzor/n8senu1r8l6h7r649ni947fqke/input?value=";
const char* host3 = "http://iot.robofun.ro/api/v1/senzor/pabu0lk5otg49jcpmko9j3tj0k/input?value=";
const char* host2 = "iot.robofun.ro";
const char* hostLocal = "http://192.168.0.108/recordTemperature.php?humidity=";
const char* hostLocal2 = "192.168.0.108";
const char* streamId   = "....................";
const char* privateKey = "....................";

float getTemperature()
{

  float temp_c;
  float temp_f;
  float humidity;

  // Read values from the sensor
  temp_c = sht1x.readTemperatureC();
  temp_f = sht1x.readTemperatureF();
  humidity = sht1x.readHumidity();

  // Print the values to the serial port
  Serial.print("Temperature: ");
  Serial.print(temp_c, DEC);
  Serial.print("C / ");
  Serial.print(temp_f, DEC);
  Serial.print("F. Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  return temp_c;
}

float getHumidity()
{

  float temp_c;
  float temp_f;
  float humidity;

  // Read values from the sensor
  temp_c = sht1x.readTemperatureC();
  temp_f = sht1x.readTemperatureF();
  humidity = sht1x.readHumidity();

  // Print the values to the serial port
  Serial.print("F. Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  return humidity;
}



void setup() {
  Serial.begin(9600);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
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

int value = 0;

void httpGetRequest(String url, String hostX)
{
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(hostX, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  Serial.print("connecting to ");
  Serial.println(hostX);

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + hostX + "\r\n" +
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

void loop() {

  ++value;


//send locally\

  // We now create a URI for the request for temperature
  String url = hostLocal;
  url += String(getHumidity());
  url += "&temperature=";
  url += String(getTemperature());
  httpGetRequest(url, hostLocal2);
  Serial.println(String(getTemperature()));




url = hostLocal;
  url += String(getHumidity());
  httpGetRequest(url, host2);

  delay(5000);
  url = host3;
  url += String(getHumidity());
  httpGetRequest(url, host2);






  delay(120000);
}

