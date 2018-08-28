#include "Clock.h"
#include "Display.h"

#include <Room.h>
#include <Log.h>
#include <LanCommunication.h>
#include <SoftwareSerial.h>

#define ADDRESS 9
#define TRIGGERED_PIN 2

#include <IRremote.h>

#define RECV_PIN  A0
IRrecv irrecv(RECV_PIN);
decode_results results;

SoftwareSerial mySerial(0, 1);
DWORD lastClockUpdate;

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

Clock clock(10);
Display display(6,//serial
                8,//shift clock
                7//storage clock
               );
void setup()
{
  mySerial.begin(9600);
  clock.InitClock();
  room.UpdateNixieClockTime();
  irrecv.enableIRIn();
}

void checkIr()
{
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
      case 16773135:
        room.SpacebarMultimedia();
        break;
      case 16732335:
        room.SwitchBenchLight();
        break;
      case 16742535:
        room.SwitchAudio();
        break;
    }
    irrecv.resume(); // Receive the next value
  }
}

void loop()
{
  DWORD displayValue;

  displayValue = 0;


  /*Serial.print(clock.GetHour());
    Serial.print(":");
    Serial.print(clock.GetMinute());
    Serial.print(":");
    Serial.println(clock.GetSecond());*/

  checkSerial();
  checkIr();

  if (500 < millis() - lastClockUpdate)
  {
    clock.UpdateTimeDate();
    displayValue = (DWORD)clock.GetHour() * 10000;
    displayValue += clock.GetMinute() * 100;
    displayValue += clock.GetSecond();
    display.Write(displayValue);

    lastClockUpdate = millis();
  }

}

void checkSerial()
{
  if (lanCom.ReadCommand())
  {
    BYTE hour;
    BYTE minute;
    BYTE second;

    hour = 0;
    minute = 0;
    second = 0;

    int *command = lanCom.GetLastCommand();
    int interval, time;
    delay(10);
    switch (command[1])
    {
      case 0:
        //Serial.println("Set time");

        hour = command[2];
        minute = command[3];
        second = command[4];

        /*Serial.print((int)hour);
          Serial.print(":");
          Serial.print((int)minute);
          Serial.print(":");
          Serial.println((int)second);
          Serial.println("Time set");*/

        clock.SetTime(hour, minute, second);
        break;

    }
  }

}


