#include <Wire.h>
#include <SoftwareSerial.h>

#define COLS 3
#define ROWS 4
#define PWM_LOCKER 200
#define ADDRESS 3

typedef enum
{
  OPEN = 0,
  CLOSE = 1
} MOTORSTATUS;

typedef enum
{
  DOOR_OPEN = 0,
  DOOR_CLOSE = 1,
  PIN_ENTERED_INCORRECTLY = 2
} LOG_TYPE;

int lockerPin1 = 5;
int lockerPin2 = 6;
int unlockerButtonPin = 15;
int lockerButtonPin = 16;
bool looked = false;
int countKeyboardPin = 0;
int keyboardColPins[] = {10, 11, 12}; //11,10,9
int keyboardRowPins[] = {7, 13, 8, 9};
int pin[] = {4, 4, 1, 2};
int adminPin[] = {6, 6, 9, 8};
int y[] = {9, 5, 6, 8};
bool isAdminLoggedIn = false;
int insertedPin[4];
int triggerPin = 2;
int speakerPin = 14;
int countInsertedPin = 0;
unsigned long now, last, interval;
int timerAction;

MOTORSTATUS motorState = OPEN;

bool isTimerOn = false;
SoftwareSerial mySerial(0, 1);
//SoftwareSerial mySerial2(11, 13);

void setTimer(int action, int duration)
{
  isTimerOn = true;
  now = millis();
  last = now;
  interval = duration;
}
void LockDoor(int source)
{
  digitalWrite(lockerPin1, LOW);
  analogWrite(lockerPin2, PWM_LOCKER);
  setTimer(0, 1000);
  motorState = CLOSE;
  sendLog(DOOR_CLOSE, source);
}
void StopMotor()
{
  digitalWrite(lockerPin1, LOW);
  digitalWrite(lockerPin2, LOW);
}
void UnlockDoor(int source)
{
  analogWrite(lockerPin1, PWM_LOCKER);
  digitalWrite(lockerPin2, LOW);
  setTimer(0, 1000);
  motorState = OPEN;
  sendLog(DOOR_OPEN, source);
}
void SwitchDoor(int source)
{
  if (OPEN == motorState)
  {
    LockDoor(source);
  }
  else if (CLOSE == motorState)
  {
    UnlockDoor(source);
  }
}
void beep()
{
  tone(speakerPin, 440, 250);
}
void notOkBeep()
{
  tone(speakerPin, 100, 250);
}
void checkTimer()
{
  if (isTimerOn)
  {
    now = millis();
    if (now - last >= interval)
    {
      isTimerOn = false;
      switch (timerAction)
      {
        case 0:
          StopMotor();
      }
    }
  }
}
int  getLinie(int pin)
{
  for (int i = 0; i < ROWS; ++i)
  {
    if (keyboardRowPins[i] == pin)
    {
      return i + 1;
    }
  }
  return 0;
}
int  getColoana(int pin)
{
  for (int i = 0; i < COLS; ++i)
  {
    if (keyboardColPins[i] == pin)
    {
      return i + 1;
    }
  }
  return 0;
}
int transforLinColToNumber(int lin, int col)
{
  return ((lin - 1) * 3) + col;
}
int getTasta(int pin1, int pin2)
{
  int col, lin;
  col = getColoana(pin1);
  lin = getLinie(pin2);
  int rez = transforLinColToNumber(lin, col);
  if (rez == 11)
    return 0;
  return rez;
}
void setup()
{
  Wire.begin(4);
  mySerial.begin(9600);
  //Serial.begin(9600);
  //mySerial2.begin(9600);
  Wire.onReceive(i2cReceiveEvent);
  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW);
  pinMode(unlockerButtonPin, INPUT_PULLUP);
  pinMode(lockerButtonPin, INPUT_PULLUP);

  for (int i = 0; i < COLS; ++i)
  {
    pinMode(keyboardColPins[i], OUTPUT);
    digitalWrite(keyboardColPins[i], HIGH);
  }
  for (int i = 0; i < ROWS; ++i)
  {
    pinMode(keyboardRowPins[i], INPUT_PULLUP);
  }

  pinMode(lockerPin1, OUTPUT);
  pinMode(lockerPin2, OUTPUT);
  pinMode(speakerPin, OUTPUT);
}
void turnOnLight(int interval, int times)
{
  int x[4];
  x[0] = 2;
  x[1] = 3;
  x[2] = interval;
  x[3] = times;
  sendCommandViaMax(x);
}
int checkAdminPin()
{
  bool ok = true;
  for (int i = 0; i < 4; ++i)
  {
    if (adminPin[i] != insertedPin[i])
    {
      ok = false;
      break;
    }
  }
  if (ok)
  {
    tone(speakerPin, 300, 250);
    isAdminLoggedIn = true;
    //return pin was correctly for log
    return 0;
  }
  else
  {
    return -1;
  }
}
void checkPin()
{
  bool ok = true;
  for (int i = 0; i < 4; ++i)
  {
    if (pin[i] != insertedPin[i])
    {
      ok = false;
      break;
    }
  }
  if (ok)
  {
    UnlockDoor(ADDRESS);
    delay(1000);
  }
  else
  {
    notOkBeep();
    //check if pin was correctly
    if (checkAdminPin())
    {
      sendLog(PIN_ENTERED_INCORRECTLY, ADDRESS);
    }
  }
  countInsertedPin = 0;
}
void pushedTasta(int number)
{
  insertedPin[countInsertedPin] = number;
  countInsertedPin++;
  if (countInsertedPin == 4)
    checkPin();
}
void sendCommandViaI2c(int address, int command)
{
  Wire.beginTransmission(address);
  Wire.write(command);
  Wire.endTransmission();
}
void sendCommandViaMax(int bytes[])
{
  digitalWrite(triggerPin, HIGH);
  delay(1);
  mySerial.write(9 + 48);
  mySerial.write(5 + 48);
  mySerial.write(6 + 48);
  mySerial.write(8 + 48);
  //mySerial.write(48+bytes[1]);
  for (int i = 0; i < 4; ++i)
  {
    mySerial.write(48 + bytes[i]);
  }
  digitalWrite(triggerPin, LOW);
  delay(1);
}
void sendOneByteViaMax(int address, int byte)
{
  int x[4];
  x[0] = address;
  x[1] = byte;
  x[2] = 0;
  x[3] = 0;
  sendCommandViaMax(x);
}
void sendLog(LOG_TYPE value, int source)
{
  int x[4];
  x[0] = 4;
  x[1] = ADDRESS;
  x[2] = value;
  x[3] = source;
  sendCommandViaMax(x);
}
void turnOnLight()
{
  sendOneByteViaMax(2, 1);
}
void turnOffLight()
{
  sendOneByteViaMax(2, 0);
}
void switchLight()
{
  sendOneByteViaMax(2, 2);
}
void multimediaPlay()
{
  sendOneByteViaMax(0, 1);
}
void multimediaPause()
{
  sendOneByteViaMax(0, 2);
}
void multimediaNext()
{
  sendOneByteViaMax(0, 3);
}
void multimediaPrevious()
{
  sendOneByteViaMax(0, 4);
}
void multimediaVolumeUp()
{
  sendOneByteViaMax(0, 5);
}
void multimediaVolumeDown()
{
  sendOneByteViaMax(0, 6);
}
void multimediaLockComputer()
{
  sendOneByteViaMax(0, 7);
}
void tastapushedAdmin(int tasta)
{
  switch (tasta)
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
      UnlockDoor(ADDRESS);
      break;
    case 5:
      LockDoor(ADDRESS);
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
      isAdminLoggedIn = false;
      break;
  }
  delay(400);
}
int x[4];
void checkSerial()
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
        if (x[0] == ADDRESS)
        {
          switch (x[1])
          {
            case 0:
              LockDoor(1);
              break;
            case 1:
              UnlockDoor(1);
              break;
            case 2:
              SwitchDoor(1);
              break;
          }
        }
      }
    }
  }
}
int getPressedTasta()
{
  for (int i = 0; i < COLS; ++i)
  {
    digitalWrite(keyboardColPins[i], LOW);
    for (int j = 0; j < ROWS; ++j)
    {
      int val = digitalRead(keyboardRowPins[j]);
      if (digitalRead(keyboardRowPins[j]) == 0)
      {
        digitalWrite(keyboardColPins[i], HIGH);
        return getTasta(keyboardColPins[i], keyboardRowPins[j]);
      }
    }
    digitalWrite(keyboardColPins[i], HIGH);
  }
  return -1;
}
void loop()
{
  //switchLight();
  //delay(100000);
  checkTimer();
  checkSerial();
  /*if (!digitalRead(unlockerButtonPin))
    {
    UnlockDoor();
    delay(1000);
    }
    if (!digitalRead(lockerButtonPin))
    {
    LockDoor();
    delay(1000);
    }*/
  int tasta = getPressedTasta();
  if (-1 != tasta)
  {
    if (!isAdminLoggedIn)
    {
      if (tasta == 12)
      {
        LockDoor(0);
        countInsertedPin = 0;
        delay(300);
      }
      else if (tasta == 10)
      {
        turnOnLight(100, 50);
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
void i2cReceiveEvent(int howMany)
{
  int x = Wire.read();
  if (x == 0)
  {
    LockDoor(0);
  }
  if (x == 1)
  {
    UnlockDoor(0);
  }
}
