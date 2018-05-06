#include <SoftwareSerial.h>
#include <Room.h>
#include <LanCommunication.h>

#define ADDRESS 1
#define TRIGGERED_PIN 2

SoftwareSerial mySerial(0, 1);

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


int maxPin = 2;
int pins[] = {10, 9, 7, 8};
int steps = 8000, step = 0;
int mode = -1;
int stepDelay = 7;
unsigned long last, now, interval;
void setup()
{
  pinMode(maxPin, OUTPUT);
  digitalWrite(maxPin, LOW);
  mySerial.begin(9600);
  for (int i = 0; i < 4; ++i)
  {
    pinMode(pins[i], OUTPUT);
  }
}
void setAllZero()
{
  for (int i = 0; i < 4; ++i)
  {
    digitalWrite(pins[i], LOW);
  }
}
void stopCurtains()
{
  digitalWrite(pins[1], 0);
  digitalWrite(pins[0], 0);
}
void closeCurtains()
{
  digitalWrite(pins[0], 0);
  digitalWrite(pins[1], 1);
  /*digitalWrite(pins[0], LOW);
    return;
    mode = 1;
    last = millis();
    interval = stepDelay;
    step = 0;*/
}
void openCurtains()
{
  digitalWrite(pins[1], 0);
  digitalWrite(pins[0], 1);
  /*analogWrite(pins[0], 200);
    return;
    mode = 0;
    last = millis();
    interval = stepDelay;
    step = 0;*/
}
void loop()
{
  //checkTimer();
  checkI2C();
}
int y[] = {9, 5, 6, 8};
int x[4];
void checkTimer()
{
  now = millis();
  if (now - last >= interval)
  {
    if (mode == 0)
    {
      int pin = step % 4;
      setAllZero();
      digitalWrite(pins[pin], 1);
      step++;
      if (step >= steps)
      {
        mode = -1;
        delay(stepDelay);
        setAllZero();
      }
    }
    if (mode == 1)
    {
      int pin = (steps - step - 1) % 4;
      setAllZero();
      digitalWrite(pins[pin], 1);
      step++;
      if (step >= steps)
      {
        mode = -1;
        delay(stepDelay);
        setAllZero();
      }
    }
    last = now;
  }
}
void checkI2C()
{
  if (lanCom.ReadCommand())
  {
    int *command = lanCom.GetLastCommand();
    int interval, time;
    delay(10);
    switch (command[2])
    {
      case 0:
        openCurtains();
        break;
      case 1:
        closeCurtains();
        break;
      case 2:
        stopCurtains();
        break;
    }
  }
}



