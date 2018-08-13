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
void loop()
{
  clock.UpdateTimeDate();
  Serial.print(clock.GetHour());
  Serial.print(":");
  Serial.print(clock.GetMinute());
  Serial.print(":");
  Serial.println(clock.GetSecond());

  display.Write(clock.GetHour() * 100 + clock.GetMinute());
}

