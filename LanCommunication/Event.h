#pragma once

#include "Types.h"
#include <Arduino.h>
#include "Time.h"

class Event
	: public Time
{
public:
	Event(DWORD interval, void(*callback)());

	void AddMiliSeconds(DWORD);

protected:

private:
	void(*_callback)();
};