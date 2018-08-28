//multimedia module
#define ADDRESS 0
#define TRIGGERED_PIN 9

#include <Room.h>
#include <Log.h>
#include <LanCommunication.h>

#include <SoftwareSerial.h>



#define WORKBENCHLIGHT_PIN 4
#define AUDIO_PIN 3
#define PIR_PIN A0
SoftwareSerial mySerial(10, 11);


bool state = false;
bool audioState = false;

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
  Serial.begin(9600);
  //setupEncj();
  
  mySerial.begin(9600);
  
  pinMode(PIR_PIN, INPUT_PULLUP);
  pinMode(WORKBENCHLIGHT_PIN, OUTPUT);
  pinMode(AUDIO_PIN, OUTPUT);
  turnOffLight(ADDRESS);
  turnOffAudio(ADDRESS);
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

void checkPir()
{
  if(digitalRead(PIR_PIN)
    && 0 == state)
    {
      Serial.println("Motion detected!");
      turnOnLight(ADDRESS);
    }
}

void loop() {
  checkSerial();
  checkPir();
  
}
void setLight(int source)
{
  Serial.print("Set light: ");
  Serial.println(state);
  digitalWrite(WORKBENCHLIGHT_PIN, !state);
  if (state)
  {
    lanLog.LightOpen(source);
  }
  else
  {
    lanLog.LightClose(source);
  }

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
  state = !state;
  setLight(source);
}

void setAudio(int source)
{
  digitalWrite(AUDIO_PIN, !audioState);
  if (audioState)
  {
    lanLog.LightOpen(source);
  }
  else
  {
    lanLog.LightClose(source);
  }

}

void turnOnAudio(int source)
{
  audioState = !false;
  setAudio(source);
}

void turnOffAudio(int source)
{
  audioState = !true;
  setAudio(source);
}

void switchAudio(int source)
{
  audioState = !audioState;
  setAudio(source);
}

void checkSerial()
{
  if (lanCom.ReadCommand())
  {
    int *command = lanCom.GetLastCommand();
    int interval, time;
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
        turnOffAudio(command[1]);
        break;
      case 4:
        turnOnAudio(command[1]);
        break;
        case 5:
        switchAudio(command[1]);
        break;

    }
  }

}






