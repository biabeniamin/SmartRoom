#include <SoftwareSerial.h>
SoftwareSerial mySerial(0, 1);
int maxPin = 2;
int pins[] = {10, 7, 9, 8};
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
  mode=-1;
}
void closeCurtains()
{
  mode = 1;
  last = millis();
  interval = stepDelay;
  step = 0;
}
void openCurtains()
{
  mode = 0;
  last = millis();
  interval = stepDelay;
  step = 0;
}
void loop()
{
  checkTimer();
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
      int pin=step%4;
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
      int pin=(steps-step-1)%4;
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
  if (mySerial.available() > 7)
  {
    while (mySerial.available())
    {
      for (int i = 0; i < 3; ++i)
      {
        x[i] = x[i + 1];
      }
      x[3] = mySerial.read() - 48;
      bool isOk = true;
      for (int i = 0; i < 4; ++i)
      {
        if (x[i] != y[i])
        {
          isOk = false;
        }
      }
      if (isOk)
      {
        for (int i = 0; i < 4; ++i)
        {
          x[i] = mySerial.read() - 48;
        }
        if (x[0] == 1)
        {
          switch (x[1])
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
    }
  }

  /**/
}


