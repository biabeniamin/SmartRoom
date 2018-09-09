#include <SoftwareSerial.h>
#include <Room.h>
#include <LanCommunication.h>
#include <Log.h>
#include <Timer.h>
#include <IRremote.h>

#define ADDRESS 8
#define TRIGGERED_PIN 2
#define RELAY_PIN 6

SoftwareSerial mySerial(0, 1);
bool state = false;
DWORD startTime;

IRsend *irsend;
Timer *timer;

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



  timer = Timer::GetInstance();
  timer->SetTimer();
  timer->TurnOn();

  irsend = new IRsend();

  pinMode(RELAY_PIN, OUTPUT);
  turnOff();
}

void turnOn()
{
  digitalWrite(RELAY_PIN, HIGH);
  state = 1;
}
void turnOff()
{
  digitalWrite(RELAY_PIN, LOW);
  state = 0;
}

void loop()
{
  checkSerial();

  if ((1 == state)
      && (3000 < timer->GetUpTime()->GetTotalMiliSeconds() - startTime))
  {
    turnOff();
    lanLog.SendLog(51, 5);
  }
}
void checkSerial()
{
  if (lanCom.ReadCommand())
  {
    int *command = lanCom.GetLastCommand();
    int interval, time;

    switch (command[2])
    {
      case 1:
        turnOn();
        startTime = timer->GetUpTime()->GetTotalMiliSeconds();
        timer->RegisterEvent(command[3] * 1000, turnOff);

        break;
      case 2:
        irsend->sendNEC (0x807E10EF, 32);
        lanLog.AcSwitch(command[1]);
        break;
    }
  }

}
