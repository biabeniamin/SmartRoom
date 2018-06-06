#include <SoftwareSerial.h>
#include <Room.h>
#include <LanCommunication.h>
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

void setup()
{
  pinMode(relayPin, OUTPUT);
  //pinMode(maxPin, OUTPUT);
  turnOffLight(ADDRESS);
  /*pinMode(triggerPin,OUTPUT);
  digitalWrite(triggerPin,LOW);*/
  //digitalWrite(maxPin, LOW);
  //Serial.begin(9600);
  mySerial.begin(9600);
}
void turnOnLight(int source)
{
  state = !false;
  setLight(source);
}
void turnOffLight(int source)
{
  state = !true;
  setLight(source);
}
void switchLight(int source)
{
  if (isTimerOn)
    state = stateBeforeTimer;
  state = !state;
  setLight(source);
}
void setLight(int source)
{
  digitalWrite(relayPin, state);
  if(state)
  {
    room.SendLog(LIGHT_OPEN,source);
  }
  else
  {
    room.SendLog(LIGHT_CLOSE,source);
  }
  
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
void stopTimer()
{
  isTimerOn = false;
}
void checkTimer()
{
  if (isTimerOn)
  {
    now = millis();
    if (now - last >= executionTime)
    {
      switch (timerAction)
      {
        case 0:
          state = !stateBeforeTimer;
          turnOffLight(timerOwner);
          break;
      }
      stopTimer();
    }
  }
}
void loop()
{
  checkTimer();
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
          irsend.sendNEC(0x807E10EF, 32);
          break;
      }
    }

}
