#include "Clock.h"
#include "Display.h"

#include <Room.h>
#include <Log.h>
#include <LanCommunication.h>
#include <SoftwareSerial.h>

#define ADDRESS 9
#define TRIGGERED_PIN 2

SoftwareSerial mySerial(3, 4);

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
Display display(5,//serial
                7,//shift clock
                6//storage clock
               );
void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  clock.InitClock();
}
DWORD aux;
void loop()
{
  DWORD displayValue;

  displayValue = 0;

  clock.UpdateTimeDate();
  Serial.print(clock.GetHour());
  Serial.print(":");
  Serial.print(clock.GetMinute());
  Serial.print(":");
  Serial.println(clock.GetSecond());

  checkSerial();

  displayValue = (DWORD)clock.GetHour() * 10000;
  displayValue += clock.GetMinute() * 100;
  displayValue += clock.GetSecond();
  display.Write(displayValue);
  delay(400);
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
    Serial.println("comm");
    switch (command[1])
    {
      case 0:
        Serial.println("Set hour");

        hour = (command[2] * 10) + command[3];
        Serial.println(hour);
        clock.SetHour(hour);
        break;
      case 1:
        Serial.println("Set minute");

        minute = (command[2] * 10) + command[3];
        Serial.println(minute);
        clock.SetMinute(minute);
        break;
      case 2:
        Serial.println("Set second");

        second = (command[2] * 10) + command[3];
        Serial.println(second);
        clock.SetSecond(second);
        break;

    }
  }

}


