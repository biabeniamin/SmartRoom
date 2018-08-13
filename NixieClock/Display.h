#pragma once
#include "Types.h"
#include "Arduino.h"

class Display
{
public:
	Display(BYTE, BYTE, BYTE);

	void Write(WORD);

private:
	void WriteCode(WORD);

	BYTE _serialPin;
	BYTE _shiftClockPin;
	BYTE _storePin;

	
};
