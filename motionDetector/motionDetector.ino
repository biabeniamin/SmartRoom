#include <WifiRoom.h>

#define SENSOR_PIN 14

char lastState;

WifiRoom *wifiRoom;
void setup()
{
  Serial.begin(9600);
  pinMode(SENSOR_PIN, INPUT);
  //attachInterrupt(digitalPinToInterrupt(SENSOR_PIN), motionDetected, RISING);
  wifiRoom = new WifiRoom();

  lastState = digitalRead(SENSOR_PIN);
  motionDetected();
  delay(1000);
  ESP.deepSleep(0);


  /*pinMode(16, OUTPUT);
    while(1)
    {
    digitalWrite(16, HIGH);
    delay(500);

    digitalWrite(16, LOW);
    delay(500);
    }*/




}

void loop()
{
  if (lastState != digitalRead(SENSOR_PIN))
  {
    lastState = digitalRead(SENSOR_PIN);
    if (1 == lastState)
    {
      motionDetected();
      delay(1000);
    }
  }

}

void motionDetected()
{
  wifiRoom->MotionDetectedStairs();
}

