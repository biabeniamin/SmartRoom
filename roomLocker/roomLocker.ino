#include <Wire.h>
#include <SoftwareSerial.h>
#include <Room.h>
#include <LanCommunication.h>

#define COLS 3
#define ROWS 4
#define PWM_LOCKER 200
#define ADDRESS 3
#define TRIGGERED_PIN 2

typedef enum
{
  OPEN = 0,
  CLOSE = 1
} MOTORSTATUS;

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
int speakerPin = 14;
int countInsertedPin = 0;
unsigned long now, last, interval;
int timerAction;

MOTORSTATUS motorState = OPEN;

bool isTimerOn = false;
SoftwareSerial mySerial(0, 1);
//SoftwareSerial mySerial2(11, 13);

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
  room.SendLog(DOOR_CLOSE, source);
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
  room.SendLog(DOOR_OPEN, source);
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
      room.SendLog(PIN_ENTERED_INCORRECTLY, ADDRESS);
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


void tastapushedAdmin(int tasta)
{
  switch (tasta)
  {
    case 1:
      room.SwitchHallLight();
      break;
    case 2:
      room.SwitchLight();
      break;
    case 3:
      room.SwitchLight();
      break;
    case 4:
      UnlockDoor(ADDRESS);
      break;
    case 5:
      LockDoor(ADDRESS);
      break;
    case 8:
      room.PlayMultimedia();
      break;
    case 7:
      room.PauseMultimedia();
      break;
    case 0:
      room.NextMultimedia();
      break;
    case 6:
      room.VolumeUpMultimedia();
      break;
    case 9:
      room.VolumeDownMultimedia();
      break;
    case 10:
      room.PreviousMultimedia();
      break;
    case 12:
      room.LockMultimedia();
      isAdminLoggedIn = false;
      break;
  }
  delay(400);
}
int x[4];
void checkSerial()
{
  if (lanCom.ReadCommand())
  {
    int *command = lanCom.GetLastCommand();
    //raspberry pi needs time to recover in order to log
    delay(100);
    switch (command[2])
    {
      case 0:
        LockDoor(command[1]);
        break;
      case 1:
        UnlockDoor(command[1]);
        break;
      case 2:
        SwitchDoor(command[1]);
        break;

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
        room.TurnOnHallLightOnSeconds(5);
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
