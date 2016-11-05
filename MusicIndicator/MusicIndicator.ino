#define ledsCount 12
struct mode
{
  int startValue;
  int step;
  int pin;
  int reads = 1;
  bool isNegative = false;
};
mode modes[2];
int leds[ledsCount] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
int currentMode = 1;
int maxPin = A2;
int y[] = {9, 5, 6, 8};
int x[4];
int networkAddress = 3;
void setup()
{
  pinMode(maxPin, OUTPUT);
  digitalWrite(maxPin, LOW);
  //music
  modes[0].startValue = 1024;
  modes[0].step = 100;
  modes[0].pin = A0;
  modes[0].reads = 5;
  modes[0].isNegative = true;
  //temperature
  modes[1].startValue = 14;
  modes[1].step = 1;
  modes[0].reads = 20;
  modes[1].pin = A1;
  for (int i = 0; i < ledsCount; ++i)
  {
    pinMode(leds[i], OUTPUT) ;
  }
  Serial.begin(9600);
}
int GetTemperature()
{
  double Temp;
  Temp = log(((10240000 / analogRead(A1)) - 10000));
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp )) * Temp );
  Temp = Temp - 273.15;// Convert Kelvin to Celcius
  Serial.println(Temp);
  return Temp;
}
int readAnalogPin(int pin)
{
  if (pin == A1)
  {
    return GetTemperature();
  }
  else
  {
    int value = analogRead(pin);
    //Serial.println(value);
    return value;
  }
}
int readValue()
{
  int value = readAnalogPin(modes[currentMode].pin) - modes[currentMode].startValue;
  if (modes[currentMode].isNegative == true)
    value = value * -1;
  //Serial.println(value);
  return value;
}
int getSteps()
{
  int steps = 0;
  for (int i = 0; i < modes[currentMode].reads; ++i)
  {
    steps += readValue() / modes[currentMode].step;
    delay(1);
  }
  steps /= modes[currentMode].reads;
  return steps;
}
void notificationReceived(int count)
{
  for (int j = 0; j < count; ++j)
  {
    for (int i = 0; i < ledsCount; ++i)
    {
      digitalWrite(leds[i], HIGH);
    }
    delay(500);
    for (int i = 0; i < ledsCount; ++i)
    {
      digitalWrite(leds[i], LOW);
    }
    delay(500);
  }
  for (int i = 0; i < ledsCount; ++i)
  {
    digitalWrite(leds[i], LOW);
  }
}
void loop()
{
  checkMax();
  int steps = getSteps();
  //Serial.println(steps);
  for (int i = 0; i < ledsCount; ++i)
  {
    if (steps > i)
    {
      digitalWrite(leds[i], HIGH);
    }
    else
    {
      digitalWrite(leds[i], LOW);
    }
  }
  delay(20);
}
void checkMax()
{
  if (Serial.available() > 7)
  {
    while (Serial.available())
    {
      for (int i = 0; i < 3; ++i)
      {
        x[i] = x[i + 1];
      }
      x[3] = Serial.read() - 48;
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
          x[i] = Serial.read() - 48;
        }
        if (x[0] == networkAddress)
        {
          switch (x[1])
          {
            case 0:
              currentMode = x[2];
              break;
            case 1:
              notificationReceived(x[2]);
              break;
          }
        }
      }
    }
  }
}
