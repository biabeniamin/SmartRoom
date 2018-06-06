//multimedia module
#define ADDRESS 0
#define TRIGGERED_PIN 9

#include <Room.h>
#include <LanCommunication.h>

#include <SoftwareSerial.h>
#include <IRremote.h>
#include <EtherCard.h>

static byte myip[] = {
  192, 168, 0, 108
};
static byte gwip[] = {
  192, 168, 0, 1
};
static byte mymac[] = {
  0x74, 0x69, 0x69, 0x2D, 0x30, 0x31
};
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
int triggerPin = 9;
SoftwareSerial mySerial(10, 11);
IRrecv irrecv(RECV_PIN);
decode_results results;

void writeLan(int byte)
{
  mySerial.write(byte);
}
int readLan()
{
  return mySerial.read();
}
int countLan()
{
  return mySerial.available();
}
LanCommunication lanCom(ADDRESS,TRIGGERED_PIN, &writeLan, &readLan, &countLan);
Room room(&lanCom);

void setupEncj()
{
  if (ether.begin(sizeof Ethernet::buffer, mymac, A0) == 0)
    Serial.println( "Failed to access Ethernet controller");
  ether.staticSetup(myip, gwip);
}
void setup()
{
  Serial.begin(9600);
  //setupEncj();
  irrecv.enableIRIn();
  mySerial.begin(9600);
}
void showNotification(int count)
{
  int x[4];
  x[0] = 3;
  x[1] = 1;
  x[2] = count;
  x[3] = 0;
  lanCom.SendCommand(x);
}
void changeLightMode(int mode)
{
  int x[4];
  x[0] = 3;
  x[1] = 0;
  x[2] = mode;
  x[3] = 0;
  lanCom.SendCommand(x);
}
void lockComputer()
{
  Keyboard.begin();
  Keyboard.press(131);
  Keyboard.print("l");
  Keyboard.releaseAll();
}

void loop() {
  //checkEncj();
  if (irrecv.decode(&results))
  {
    switch (results.value)
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
      room.CloseDoor();
      break;
    case 2327013275:
      room.OpenDoor();
      break;
    case 3691091931:
      room.OpenCurtains();
      break;
    case 1153697755:
      room.CloseCurtains();
      break;
    case 4034314555:
      room.StopCurtains();
      break;
    case 2980520219:
      room.TurnOnLight();
      break;
    case 3961599383:
      room.TurnOffLight();
      break;
    case 1217346747:
      room.SwitchLight();
      break;
    case 4287727287:
      room.TurnOnHallLight();
      break;
    case 4131161687:
      room.TurnOffHallLight();
      break;
    case 324312031:
      room.SwitchHallLight();
      break;
    case 3577243675:
      lockComputer();
      break;
    case 3855596927:
      room.TurnOnLightOnSeconds(1);
      break;
    case 2721879231:
      room.TurnOnLightOnSeconds(2);
      break;
    case 3877748955:
      room.TurnOnLightOnSeconds(3);
      break;
    case 4039382595:
      room.TurnOnLightOnSeconds(4);
      break;
    case 2538093563:
      room.TurnOnLightOnSeconds(5);
      break;
    case 2259740311:
      room.TurnOnLightOnSeconds(6);
      break;
    case 2666828831:
      room.TurnOnLightOnSeconds(7);
      break;
    case 2747854299:
      room.TurnOnLightOnSeconds(8);
      break;
    case 1541889663:
      room.TurnOnLightOnSeconds(9);
      break;
    /*case 4287727287:
      showNotification(5);
      break;*/
    case 3109255487:
      room.SwitchAc( );
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
  switch (command)
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








