#include "Event.h"

Event::Event(DWORD interval, void(*callback)())
	: Time(interval)
{
	_callback = callback;
}

void Event::AddMiliSeconds(DWORD MiliSeconds)
{
	if ((0 == _miliSeconds) || (0 >= _miliSeconds + MiliSeconds))
	{
		_miliSeconds = 0;

		_callback();
	}
	else
	{
		_miliSeconds += MiliSeconds;
	}
}