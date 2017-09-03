int count=0;
int opAmpInputPin=6;
int colorDelay[]={3000,200,3};
// the setup function runs once when you press reset or power the board
void setup()
{
  pinMode(6, OUTPUT);
  Serial.begin(9600);
}
int displayColor(int color,unsigned long duration)
{
  unsigned long last,interval,count=0,steps,beginning;
  int lastState=0;
  steps=duration*1000/colorDelay[color];
  interval=colorDelay[color];
  Serial.print("Interval:");
  Serial.println(interval);
  last=micros();
  beginning=last;
  while(count<steps && micros()-beginning<duration*1000)
  {
    if(micros()-last>interval)
    {
      if(lastState==0)
      {
        digitalWrite(opAmpInputPin, HIGH);
        lastState=1;
      }
      else if(lastState==1)
      {
        digitalWrite(opAmpInputPin, LOW);
        lastState=0;
      }
      count++;
      last=micros();
    }
  }
  /*for(unsigned long i=0;i<duration*500/colorDelay[color];++i)
  {
    digitalWrite(opAmpInputPin, HIGH);
    delayMicroseconds(colorDelay[color]);
    digitalWrite(opAmpInputPin, LOW);
    delayMicroseconds(colorDelay[color]);
  }*/
}
void loop()
{
  displayColor(0,4000);
  delay(1000);
  displayColor(1,4000);
  delay(1000);
  displayColor(2,4000);
  delay(1000);
}
