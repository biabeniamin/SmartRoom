#pragma once

#include "Types.h"
#include <Arduino.h>
#include "ITime.h"

class Time
	: public ITime
{
public:
	Time();
	Time(DWORD);
	
	BYTE GetHours();
	BYTE GetMinutes();
	BYTE GetSeconds();
	BYTE GetMiliSeconds();

	DWORD GetTotalSeconds();
	DWORD GetTotalMiliSeconds();

	void AddMiliSeconds(DWORD);

	void SetTime(DWORD);

protected:
	DWORD _miliSeconds;
};