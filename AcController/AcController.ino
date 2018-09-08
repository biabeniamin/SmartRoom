#include <SoftwareSerial.h>
#include <Room.h>
#include <LanCommunication.h>
#include <Log.h>
#include <Timer.h>
#include <IRremote.h>

#define ADDRESS 8
#define TRIGGERED_PIN 9

SoftwareSerial mySerial(10, 11);
int relayPin = 13;
bool switcherState;
bool state = false;
unsigned long now, last, executionTime;
bool isTimerOn = false;
int timerAction = -1;
int timerOwner = 0;
bool stateBeforeTimer = false;

//IRsend irsend;
Timer *timer;

void writeLan(int byte)
{
  mySerial.write(byte);
}
int readLan()
{
  /*int x = mySerial.read();
    Serial.println(x);
    return x;*/
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
  Serial.begin(9600);
  delay(1000);
  Serial.println("Start");


  timer = Timer::GetInstance();
  timer->SetTimer();
  timer->TurnOn();
  pinMode(13, OUTPUT);
  turnOff();
}

void turnOn()
{
  digitalWrite(13, HIGH);
}
void turnOff()
{
  Serial.println("close");
  digitalWrite(13, LOW);
}

void loop()
{
  checkSerial();
}
void checkSerial()
{
  if (lanCom.ReadCommand())
  {
    Serial.println("comm");
    Serial.print(timer->GetUpTime()->GetTotalSeconds());
    turnOn();
    timer->RegisterEvent(60000, turnOff);

    int *command = lanCom.GetLastCommand();
    int interval, time;
    delay(10);
    switch (command[2])
    {
      case 2:
        // irsend.sendNEC (0x807E10EF, 32);
        lanLog.AcSwitch(command[1]);
        break;
    }
  }

}
