//multimedia module
#define ADDRESS 0
#define TRIGGERED_PIN 9

#include <Room.h>
#include <Log.h>
#include <LanCommunication.h>

#include <SoftwareSerial.h>
#include <IRremote.h>

#define RECV_PIN  5
#define WORKBENCHLIGHT_PIN 4
#define AUDIO_PIN 3
#define PIR_PIN A0
SoftwareSerial mySerial(10, 11);
IRrecv irrecv(RECV_PIN);
decode_results results;

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
  irrecv.enableIRIn();
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
  if (irrecv.decode(&results))
  {
    switch (results.value)
    {
      case 16752735:
        room.VolumeUpMultimedia();
        break;
      case 16711935:
        room.VolumeDownMultimedia();
        break;
      case 16724175:
        room.VolumeMuteMultimedia();
        break;
      case 16718055:
        room.NextMultimedia();
        break;
      case 16762935:
        room.PreviousMultimedia();
        break;
      case 16773645:
        room.PlayPauseMultimedia();
        break;
      case 16771605:
        room.PlayPauseMultimedia();
        break;
      case 16713975:
        room.CloseDoor();
        break;
      case 16744575:
        room.OpenDoor();
        break;
      case 16734375:
        room.OpenCurtains();
        break;
      case 16740495:
        room.CloseCurtains();
        break;
      case 16769055:
        room.StopCurtains();
        break;
      case 16767525:
        room.TurnOnLight();
        break;
      case 16765485:
        room.TurnOffLight();
        break;
      case 16726215:
        room.SwitchLight();
        break;
      case 16722135:
        room.SwitchHallLight();
        break;
      case 16764975:
        room.LockMultimedia();
        break;
      case 16748655:
        room.TurnOnLightOnSeconds(1);
        break;
      case 16758855:
        room.TurnOnLightOnSeconds(2);
        break;
      case 16775175:
        room.TurnOnLightOnSeconds(3);
        break;
      case 16756815:
        room.TurnOnLightOnSeconds(4);
        break;
      case 16750695:
        room.TurnOnLightOnSeconds(5);
        break;
      case 16767015:
        room.TurnOnLightOnSeconds(6);
        break;
      case 16746615:
        room.TurnOnLightOnSeconds(7);
        break;
      case 16754775:
        room.TurnOnLightOnSeconds(8);
        break;
      case 16771095:
        room.TurnOnLightOnSeconds(9);
        break;
      /*case 4287727287:
        showNotification(5);
        break;*/
      case 16763445:
        room.SwitchAc( );
        break;
      case 1337545183:
        changeLightMode(0);
        break;
      case 16773135:
        room.SpacebarMultimedia();
        break;
      case 16732335:
        switchLight(ADDRESS);
        break;
        case 16742535:
        switchAudio(ADDRESS);
        break;
    }
    Serial.println(results.value);
    irrecv.resume(); // Receive the next value
  }
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






