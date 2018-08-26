#include "Display.h"

#define GET_BIT(X, N) ((X >> N) & 0b1)
#define SECOND_DISPLAY_BITS(X) ((GET_BIT(X, 0) << 2) | (GET_BIT(X, 1) << 1) | (GET_BIT(X, 2) << 3) | (GET_BIT(X, 3) << 0))
#define SECOND_DISPLAY(X) (((X & (0b1 << 3)) << 4) | (X & 0b111))

int _digits[] = {
  0b1000,//0
  0b0000,//1
  0b0100,//2
  0b0110,//3
  0b0010,//4
  0b0011,//5
  0b0111,//6
  0b0101,//7
  0b0001,//8
  0b1100,//9
};

int _digitsFirstDisplay[] = {
  0b0001,//0
  0b1000,//1
  0b0100,//2
  0b0010,//3
  0b0000,//4
  0b0000,//5
  0b0000,//6
  0b0000,//7
  0b0000,//8
  0b0000,//9
};

Display::Display(BYTE SerialPin, BYTE ShiftClockPin, BYTE StorePin)
{
  _serialPin = SerialPin;
  _shiftClockPin = ShiftClockPin;
  _storePin = StorePin;

  pinMode(_serialPin, OUTPUT);
  pinMode(_shiftClockPin, OUTPUT);
  pinMode(_storePin, OUTPUT);
}

void Display::Write(DWORD Value)
{
  DWORD code;

  code = 0;

  code = SECOND_DISPLAY(SECOND_DISPLAY_BITS(_digits[Value % 10]));
  Value /= 10;

  code |= _digits[Value % 10] << 3;
  Value /= 10;

  code = code << 8;

  code |= SECOND_DISPLAY(SECOND_DISPLAY_BITS(_digits[Value % 10]));
  Value /= 10;

  code |= _digits[Value % 10] << 3;
  Value /= 10;

  code = code << 8;

  //third display

  code |= SECOND_DISPLAY(_digitsFirstDisplay[Value / 10 % 10]);

  code |= _digits[Value % 10] << 3;
  Value /= 100;

  

  WriteCode(code);
}

void Display::WriteCode(DWORD Value)
{
  for (int i = 0; i < 24; i++)
  {

    digitalWrite(_serialPin, Value & 0x1);
    Value >>= 1;

    digitalWrite(_shiftClockPin, HIGH);
    digitalWrite(_shiftClockPin, LOW);

  }
  digitalWrite(_storePin, HIGH);
  delay(1);
  digitalWrite(_storePin, LOW);

  delay(1);
}
