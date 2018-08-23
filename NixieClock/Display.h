#pragma once
#include "Types.h"
#include "Arduino.h"

class Display
{
public:
	Display(BYTE, BYTE, BYTE);

	void Write(DWORD);

private:
	void WriteCode(DWORD);

	BYTE _serialPin;
	BYTE _shiftClockPin;
	BYTE _storePin;

	
};
