// A simple web server that always just says "Hello World"

#include "etherShield.h"
#include "ETHER_28J60.h"

int outputPin = 6;

static uint8_t mac[6] = {0x54, 0x55, 0x58, 0x10, 0x00, 0x24};   // this just needs to be unique for your network, 
                                                                // so unless you have more than one of these boards
                                                                // connected, you should be fine with this value.
                                                           
static uint8_t ip[4] = {192, 168, 0, 115};                       // the IP address for your board. Check your home hub
                                                                // to find an IP address not in use and pick that
                                                                // this or 10.0.0.15 are likely formats for an address
                                                                // that will work.

static uint16_t port = 80;                                      // Use port 80 - the standard for HTTP

ETHER_28J60 e;

void setup()
{ 
  e.setup(mac, ip, port);
  pinMode(outputPin, OUTPUT);
}
void loop()
{
  char* params;
  if (params = e.serviceRequest())
  {
    e.print("<H1>Cel mai tare mod de a controla lumina</H1><br>Lumina actuala in camera este de:");
    e.print("<progress value= ");
    e.print(analogRead(0));
    e.print(" max=1024></progress>");
    e.print(analogRead(0));
   if(strcmp(params, "?cmd=switch") == 0)
    {
//      e.print("<A HREF='?cmd=on'>Turn on</A><br>");
      bool value=digitalRead(outputPin);
      digitalWrite(outputPin,!value);
      if(value)
      {
        e.print("T");
      }
      else
      {
        e.print("F");
      }
    }   
    e.respond();
  }
}

