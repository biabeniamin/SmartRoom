#include "Clock.h"
#include "Display.h"

Clock clock(10);
Display display(5,//serial
  7,//shift clock
  6//storage clock
  );
void setup()
{
  Serial.begin(9600);
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

  displayValue = (DWORD)clock.GetHour() * 10000;
  displayValue += clock.GetMinute() * 100;
  displayValue += clock.GetSecond();
  display.Write(displayValue);
  delay(100);
}

