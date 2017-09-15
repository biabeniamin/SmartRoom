#include <SoftwareSerial.h>
#include <Room.h>
#include <LanCommunication.h>

#define ADDRESS 2
#define TRIGGERED_PIN A3

SoftwareSerial mySerial(0, 1);
int triggerPin = 2;
int relayPin = 9;
int switcherPin = 8;
int maxPin = 2;
bool switcherState;
bool state = false;
unsigned long now, last, executionTime;
bool isTimerOn = false;
int timerAction = -1;
int timerOwner=0;
bool stateBeforeTimer = false;

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
  pinMode(switcherPin, INPUT_PULLUP);
  switcherState = digitalRead(switcherPin);
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
void checkSwitcher()
{
  if (digitalRead(switcherPin) != switcherState)
  {
    switchLight(ADDRESS);
    switcherState = digitalRead(switcherPin);
    stopTimer();
    delay(300);
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
  //checkSwitcher();
  //checkTimer();
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
        case 0:
          turnOffLight(command[1]);
          break;
        case 1:
          turnOnLight(command[1]);
          break;
        case 2:
          switchLight(command[1]);
          break;
        case 3:
          interval = command[3];
          time = 1;//command[3];
          initializingTimer((interval * time) * 1000, 0,command[1]);
          turnOnLight(command[1]);
          break;
      }
    }

}
