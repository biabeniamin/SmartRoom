#include <Wire.h>
#include <SoftwareSerial.h>
struct keyboardPin
{
  int digitalPin;
  int pin;
};
keyboardPin pins[7];
int lockerPin1=9;
int lockerPin2=10;
int unlockerButtonPin=15;
int lockerButtonPin=16;
bool looked=false;
int countKeyboardPin=0;
int pin[]={4,4,1,2};
int adminPin[]={6,6,9,8};
int y[]={9,5,6,8};
bool isAdminLoggedIn=false;
int insertedPin[4];
int triggerPin=12;
int speakerPin=14;
int countInsertedPin=0;
unsigned long now,last,interval;
int timerAction;
bool isTimerOn=false;
//SoftwareSerial mySerial(0, 1);
SoftwareSerial mySerial2(11, 13);
void addPin(int digitalPin,int pin)  
{
  
  pins[countKeyboardPin].digitalPin=digitalPin;
  pins[countKeyboardPin].pin=pin;
  countKeyboardPin++;
}
int  getLinie(int pin)
{
  int line=-1;
  switch(pin)
  {
    case 2:
      line=1;
      break;
    case 7:
      line=2;
      break;
    case 6:
      line=3;
      break;
    case 4:
      line=4;
      break;
  }
  return line;
}
int  getColoana(int pin)
{
  int col=-1;
  switch(pin)
  {
    case 3:
      col=1;
      break;
    case 1:
      col=2;
      break;
    case 5:
      col=3;
      break;
  }
  return col;
}
int transforLinColToNumber(int lin,int col)
{
  return ((lin-1)*3)+col;
}
int getTasta(int pin1,int pin2)
{
  int col,lin;
  col=getColoana(pin1);
  if(col==-1)
    col=getColoana(pin2);
  lin=getLinie(pin1);
  if(lin==-1)
    lin=getLinie(pin2);
  int rez=transforLinColToNumber(lin,col);
  if(rez==11)
    return 0;
  return rez;
}
void setTimer(int action,int duration)
{
  isTimerOn=true;
  now=millis();
  last=now;
  interval=duration;
}
void LockDoor()
{
  digitalWrite(lockerPin1,LOW);
  digitalWrite(lockerPin2,HIGH);
  setTimer(0,1000);
}
void StopMotor()
{
  digitalWrite(lockerPin1,LOW);
  digitalWrite(lockerPin2,LOW);
}
void UnlockDoor()
{
  digitalWrite(lockerPin1,HIGH);
  digitalWrite(lockerPin2,LOW);
  setTimer(0,1000);
}
void beep()
{
  tone(speakerPin,440,250);
}
void notOkBeep()
{
  tone(speakerPin,100,250);
}
void checkTimer()
{
  if(isTimerOn)
  {
    now=millis();
    if(now-last>=interval)
    {
      isTimerOn=false;
      switch(timerAction)
      {
        case 0:
          StopMotor();
      }
    }
  }
}
void setup()
{
  Wire.begin(4);
  //mySerial.begin(9600);
  Serial.begin(9600);
  mySerial2.begin(9600);
  Wire.onReceive(i2cReceiveEvent);
  addPin(2,1);
  addPin(3,2);
  addPin(4,3);
  addPin(5,4);
  addPin(6,5);
  addPin(7,6);
  addPin(8,7);
  pinMode(triggerPin,OUTPUT);
  digitalWrite(triggerPin,LOW);
  pinMode(unlockerButtonPin,INPUT_PULLUP);
  pinMode(lockerButtonPin,INPUT_PULLUP);
  for(int i=0;i<countKeyboardPin;++i)
  {
    if(getLinie(pins[i].pin)!=-1)
    {
      pinMode(pins[i].digitalPin,OUTPUT);
      digitalWrite(pins[i].digitalPin,HIGH);
    }
    else
      pinMode(pins[i].digitalPin,INPUT_PULLUP);
  }
  pinMode(lockerPin1,OUTPUT);
  pinMode(lockerPin2,OUTPUT);
  pinMode(speakerPin,OUTPUT);
}
void turnOnLight(int interval,int times)
{
  int x[4];
  x[0]=2;
  x[1]=3;
  x[2]=interval;
  x[3]=times;
  sendCommandViaMax(x);
}
void checkAdminPin()
{
  bool ok=true;
  for(int i=0;i<4;++i)
  {
    if(adminPin[i]!=insertedPin[i])
    {
      ok=false;
      break;
    }
  }
  if(ok)
  {
    tone(speakerPin,300,250);
    isAdminLoggedIn=true;
  }
}
void checkPin()
{
  bool ok=true;
  for(int i=0;i<4;++i)
  {
    if(pin[i]!=insertedPin[i])
    {
      ok=false;
      break;
    }
  }
  if(ok)
  {
    UnlockDoor();
    delay(1000);
  }
  else
  {
    notOkBeep();
    checkAdminPin();
  }
  countInsertedPin=0;
}
void pushedTasta(int number)
{
  insertedPin[countInsertedPin]=number;
  countInsertedPin++;
  if(countInsertedPin==4)
    checkPin();
}
void sendCommandViaI2c(int address,int command)
{
  Wire.beginTransmission(address);
  Wire.write(command);
  Wire.endTransmission();
}
void sendCommandViaMax(int bytes[])
{
  digitalWrite(triggerPin,HIGH);
  delay(1);
  //mySerial.write(48+bytes[1]);
  for(int i=0;i<4;++i)
  {
    Serial.write(48+bytes[i]);
    mySerial2.print("    ");
    mySerial2.print(bytes[i]);
    mySerial2.print("    ");
  }
  mySerial2.println("    ");
  digitalWrite(triggerPin,LOW);
  delay(1);
}
void sendCommandViaMax(int address,int byte)
{
  int x[4];
  x[0]=address;
  x[1]=byte;
  x[2]=0;
  x[3]=0;
  sendCommandViaMax(x);
}
void turnOnLight()
{
  sendCommandViaMax(2,1);
}
void turnOffLight()
{
  sendCommandViaMax(2,0);
}
void switchLight()
{
  sendCommandViaMax(2,2);
}
void multimediaPlay()
{
  sendCommandViaMax(0,1);
}
void multimediaPause()
{
  sendCommandViaMax(0,2);
}
void multimediaNext()
{
  sendCommandViaMax(0,3);
}
void multimediaPrevious()
{
  sendCommandViaMax(0,4);
}
void multimediaVolumeUp()
{
  sendCommandViaMax(0,5);
}
void multimediaVolumeDown()
{
  sendCommandViaMax(0,6);
}
void multimediaLockComputer()
{
  sendCommandViaMax(0,7);
}
void tastapushedAdmin(int tasta)
{
  switch(tasta)
  {
    case 1:
      turnOnLight();
      break;
   case 2:
      turnOffLight();
      break;
   case 3:
      switchLight();
      break;
   case 4:
      UnlockDoor();
      break;
   case 5:
      LockDoor();
      break;
   case 8:
     multimediaPlay();
     break;
   case 7:
     multimediaPause();
     break;
   case 0:
     multimediaNext();
     break;
   case 6:
     multimediaVolumeUp();
     break;
   case 9:
     multimediaVolumeDown();
     break;  
   case 10:
     multimediaPrevious();
     break;
   case 12:
     multimediaLockComputer();
     isAdminLoggedIn=false;
     break;
  }
  delay(400);
}
int x[4];
void checkSerial()
{
  if (Serial.available()>7)
  {
    while(Serial.available())
    {
      for(int i=0;i<3;++i)
      {
        x[i]=x[i+1];
      }
      x[3]=Serial.read()-48;
      bool isOk=true;
      for(int i=0;i<4;++i)
      {
        if(x[i]!=y[i])
        {
          isOk=false;
        }
      }
      if(isOk)
      {
        for(int i=0;i<4;++i)
        {
          x[i]=Serial.read()-48;
          mySerial2.write(x[i]+48);
        }
        mySerial2.println();
        if(x[0]==1)
        {
          switch(x[1])
          {
            case 0:
              LockDoor();
              break;
            case 1:
              UnlockDoor();
              break;
          }
        }
      }
    }
  }
}
void loop()
{
  checkTimer();
  checkSerial();
  if(!digitalRead(unlockerButtonPin))
  {
    UnlockDoor();
    delay(1000);
  }
  if(!digitalRead(lockerButtonPin))
  {
    LockDoor();
    delay(1000);
  }
  for(int i=0;i<countKeyboardPin;++i)
  {
    if(getLinie(pins[i].pin)==-1)
      continue;
    digitalWrite(pins[i].digitalPin,LOW);
    for(int j=0;j<countKeyboardPin;++j)
    {
      if(j!=i)
      {
        if(!digitalRead(pins[j].digitalPin))
        {
          int tasta=getTasta(pins[j].pin,pins[i].pin);
          if(!isAdminLoggedIn)
          {
            if(tasta==12)
            {
              LockDoor();
              countInsertedPin=0;
            }
            else if(tasta==10)
            {
              turnOnLight(100,50);
              delay(300);
            }
            else
            {
              beep();
              pushedTasta(tasta);
              delay(300);
            }
          }
          else
          {
            tastapushedAdmin(tasta);
          } 
        }
      }
    }
    digitalWrite(pins[i].digitalPin,HIGH);
  }
}
void i2cReceiveEvent(int howMany)
{
  int x = Wire.read();
  if(x==0)
  {
    LockDoor();
  }
  if(x==1)
  {
    UnlockDoor();
  }
}
