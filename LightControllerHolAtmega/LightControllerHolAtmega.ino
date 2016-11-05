#include <SoftwareSerial.h>
SoftwareSerial mySerial(5,6);
int triggerPin=2;
int relayPin=9;
int switcherPin=8;
int maxPin=A3;
bool switcherState;
bool state=false;
unsigned long now,last,executionTime;
bool isTimerOn=false;
int timerAction=-1;
bool stateBeforeTimer=false;
void setup()
{
  pinMode(relayPin,OUTPUT);
  pinMode(maxPin,OUTPUT);
  pinMode(switcherPin,INPUT_PULLUP);
  switcherState=digitalRead(switcherPin);
  turnOffLight();
  /*pinMode(triggerPin,OUTPUT);
  digitalWrite(triggerPin,LOW);*/
  digitalWrite(maxPin,LOW);
  Serial.begin(9600);
  mySerial.begin(9600);
}

void turnOnLight()
{
  state=!false;
  setLight();
}
void turnOffLight()
{
  state=!true;
  setLight();
}
void switchLight()
{
  if(isTimerOn)
    state=stateBeforeTimer;
  state=!state;
  setLight();
}
void setLight()
{
  digitalWrite(relayPin,state);
}
void checkSwitcher()
{
  if(digitalRead(switcherPin)!=switcherState)
  {
    switchLight();
    switcherState=digitalRead(switcherPin);
    stopTimer();
    delay(300);
  }
}
void initializingTimer(int duration,int action)
{
  last=millis();
  executionTime=duration;
  isTimerOn=true;
  stateBeforeTimer=state;
  timerAction=action;
}
void stopTimer()
{
  isTimerOn=false;
}
void checkTimer()
{
  if(isTimerOn)
  {
    now=millis();
    if(now-last>=executionTime)
    {
      switch(timerAction)
      {
        case 0:
          state=!stateBeforeTimer;
          turnOffLight();
          break;
      }
      stopTimer();
    }
  }
}
void loop()
{
  checkSwitcher();
  checkTimer();
  checkI2C();
}
int y[]={9,5,6,8};
int x[4];
void checkI2C()
{
  if (Serial.available()>7)
  {
    while(Serial.available())
    {
      for(int i=0;i<3;++i)
      {
        x[i]=x[i+1];
      }
      x[3]=Serial.read()-48;
      bool isOk=true;
      for(int i=0;i<4;++i)
      {
        if(x[i]!=y[i])
        {
          isOk=false;
        }
      }
      if(isOk)
      {
        for(int i=0;i<4;++i)
        {
          x[i]=Serial.read()-48;
          mySerial.write(x[i]+48);
        }
        mySerial.println();
        if(x[0]==3)
        {
         int time,interval;
          stopTimer();
          switch(x[1])
          {
            case 0:
              turnOffLight();
              break;
            case 1:
              turnOnLight();
              break;
            case 2:
              switchLight();
              break;
            case 3:
              interval=x[2];
              time=x[3];
              initializingTimer((interval*time)*1000,0);
              turnOnLight();
              break;
          }
        }
      }
    }
  }
  
  /**/
}
