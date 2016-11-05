#include <SoftwareSerial.h>
#include <IRremote.h>
#include <EtherCard.h>
static byte myip[] = { 
  192, 168, 0, 108 };
static byte gwip[] = { 
  192, 168, 0, 1 };
static byte mymac[] = { 
  0x74, 0x69, 0x69, 0x2D, 0x30, 0x31 };
byte Ethernet::buffer[1000];
const char page[] PROGMEM =
"<html>"
"<head><title>"
"Beniamin Bia's Room Controller"
"</title></head>"
"<body style='background-color:blue'>"
"<div style='background-color:#00ccff;width:400px;margin:auto;'>;"
"<center><H1>Beniamin Bia's Room Controller</H1>"
"<a href='?cmd=1'><button>Switch Light</button></a> "
"<a href='?cmd=2'><button>Turn Off Light</button></a> "
"<a href='?cmd=3'><button>Close Door</button></a> "
"<a href='?cmd=4'><button>Open Door</button></a> "
"<a href='?cmd=5'><button>Music Mode</button></a> "
"<a href='?cmd=6'><button>Temperature Mode</button></a> "
"<a href='?cmd=7'><button>Notification</button></a> "

"<center></div></body>"
"</html>"
;

int RECV_PIN = 5;
int triggerPin=9;
SoftwareSerial mySerial(10, 11);
IRrecv irrecv(RECV_PIN);
decode_results results;
void setupEncj()
{
  if (ether.begin(sizeof Ethernet::buffer, mymac, A0) == 0)
    Serial.println( "Failed to access Ethernet controller");
  ether.staticSetup(myip, gwip);
}
void setup()
{
  Serial.begin(9600);
  setupEncj();
  irrecv.enableIRIn();
  mySerial.begin(9600);
  pinMode(triggerPin,OUTPUT);
  digitalWrite(triggerPin,LOW);

}
void sendCommandViaMax(int bytes[])
{
  digitalWrite(triggerPin,HIGH);
  delay(1);
  mySerial.write(9+48);
  mySerial.write(5+48);
  mySerial.write(6+48);
  mySerial.write(8+48);
  //mySerial.write(48+bytes[1]);
  for(int i=0;i<4;++i)
  {
    mySerial.write(48+bytes[i]);
    Serial.print("    ");
    Serial.print(bytes[i]);
    Serial.print("    ");
  }
  Serial.println("    ");
  digitalWrite(triggerPin,LOW);
  delay(1);
}
void sendOneByteViaMax(int address,int byte)
{
  int x[4];
  x[0]=address;
  x[1]=byte;
  x[2]=0;
  x[3]=0;
  sendCommandViaMax(x);
}
void closeDoor()
{
  sendOneByteViaMax(1,0);
}
void openDoor()
{
  sendOneByteViaMax(1,1);
}
void turnOnLight()
{
  sendOneByteViaMax(2,1);
}
void turnOffLight()
{
  sendOneByteViaMax(2,0);
}
void switchLight()
{
  sendOneByteViaMax(2,2);
}
void showNotification(int count)
{
  int x[4];
  x[0]=3;
  x[1]=1;
  x[2]=count;
  x[3]=0;
  sendCommandViaMax(x);
}
void changeLightMode(int mode)
{
  int x[4];
  x[0]=3;
  x[1]=0;
  x[2]=mode;
  x[3]=0;
  sendCommandViaMax(x);
}
void turnOnLight(int seconds)
{
  int x[4];
  x[0]=2;
  x[1]=3;
  x[2]=seconds;
  x[3]=1;
  sendCommandViaMax(x);
}
void lockComputer()
{
  Keyboard.begin();
  Keyboard.press(131);
  Keyboard.print("l");
  Keyboard.releaseAll();
}
void checkEncj()
{
  word pos = ether.packetLoop(ether.packetReceive());
  char* request = "GET /?cmd=1 ";
  if (pos)
  {
    char* data = (char *) Ethernet::buffer + pos;
    for (int i = 0; i < 10; ++i)
    {
      request[10] = i + 48;
      if (strncmp(request, data, strlen(request)) == 0)
      {
        switch(i)
        {
        case 1:
          switchLight();
          break;
        case 2:
          turnOffLight();
          break;
        case 3:
          closeDoor();
          break;
        case 4:
          openDoor();
          break;
        case 5:
          changeLightMode(0);
          break;
        case 6:
          changeLightMode(1);
          break;
        case 7:
          showNotification(3);
          break;

        }
      }
    }
    memcpy_P(ether.tcpOffset(), page, sizeof page);
    ether.httpServerReply(sizeof page - 1);
  }
}
void loop() {
  checkEncj();
  if (irrecv.decode(&results)) 
  {
    switch(results.value)
    {
    case 2878444831:
      Remote.increase();
      break;
    case 4198438303:
      Remote.decrease();
      break;
    case 2534850111:
      Remote.mute();
      break;
    case 1033561079:
      Remote.next();
      break;
    case 2388475579:
      Remote.previous();
      break;
    case 2318624347:
      Remote.play();
      break;
    case 2797888379:
      Remote.pause();
      break;
    case 1162296347:
      closeDoor();
      break;
    case 2327013275:
      openDoor();
      break;
    case 2980520219:
      turnOnLight();
      break;
    case 3961599383:
      turnOffLight();
      break;
    case 324312031:
      switchLight();
      break;
    case 3577243675:
      lockComputer();
      break;
    case 3855596927:
      turnOnLight(1);
      break;
    case 2721879231:
      turnOnLight(2);
      break;
    case 3877748955:
      turnOnLight(3);
      break;
    case 4039382595:
      turnOnLight(4);
      break;
    case 2538093563:
      turnOnLight(5);
      break;
    case 2259740311:
      turnOnLight(6);
      break;
    case 2666828831:
      turnOnLight(7);
      break;
    case 2747854299:
      turnOnLight(8);
      break;
    case 1541889663:
      turnOnLight(9);
      break;
    case 4287727287:
      showNotification(5);
      break; 
    case 3109255487:
      changeLightMode(1);
      break; 
    case 1337545183:
      changeLightMode(0);
      break;      
    }
    Serial.println(results.value);
    irrecv.resume(); // Receive the next value
    Remote.clear();
  }
}
void remoteCommand(int command)
{
  switch(command)
  {
    //case 6:lockComputer();break;
  case 2:
    Remote.next();
    break;
  case 3:
    Remote.previous();
    break;
  case 1:
    Remote.play();
    break;
  case 0:
    Remote.pause();
    break;
  case 5:
    Remote.increase();
    delay(100);
    break;
  case 4:
    Remote.decrease();
    delay(100);
    break;
  case 7:
    Remote.mute();
    break;
  }
  Remote.clear();
}
void receiveEventI2c(int howMany)
{
  //int x=Wire.read();
  //remoteCommand(x);
}






