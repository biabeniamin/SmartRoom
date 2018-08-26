#include <SoftwareSerial.h>
#include <Room.h>
#include <LanCommunication.h>
#include <Log.h>
#include <IRremote.h>

#define ADDRESS 8
#define TRIGGERED_PIN 2

SoftwareSerial mySerial(0, 1);
int relayPin = 13;
bool switcherState;
bool state = false;
unsigned long now, last, executionTime;
bool isTimerOn = false;
int timerAction = -1;
int timerOwner=0;
bool stateBeforeTimer = false;

IRsend irsend;

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

LanCommunication lanCom(ADDRESS, TRIGGERED_PIN, &writeLan, &readLan, &countLan);
Room room(&lanCom);
Log lanLog(&lanCom);

void setup()
{
  mySerial.begin(9600);
}

void initializingTimer(int duration, int action,int owner)
{
  last = millis();
  executionTime = duration;
  isTimerOn = true;
  stateBeforeTimer = state;
  timerAction = action;
  timerOwner=owner;
}

void loop()
{
  checkSerial();
}
void checkSerial()
{
    if (lanCom.ReadCommand())
    {
      int *command = lanCom.GetLastCommand();
      int interval,time;
      delay(10);
      switch (command[2])
      {
        case 2:
          irsend.sendNEC (0x807E10EF, 32);
          lanLog.AcSwitch(command[1]);
          break;
      }
    }

}
