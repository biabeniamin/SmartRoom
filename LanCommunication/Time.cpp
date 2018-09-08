#include "Time.h"

Time::Time()
	: Time(0)
{
	
}

Time::Time(DWORD Miliseconds)
{
	_miliSeconds = Miliseconds;
}

BYTE Time::GetHours()
{
	return ((_miliSeconds / (1000ul * 60 * 60)) % 100);
}

BYTE Time::GetMinutes()
{
	return (_miliSeconds / (1000ul * 60)) % 60;
}

BYTE Time::GetSeconds()
{
	return (_miliSeconds / (1000ul)) % 60;
}

BYTE Time::GetMiliSeconds()
{
	return (_miliSeconds) % 1000;
}

DWORD Time::GetTotalSeconds()
{
	return (_miliSeconds / (1000ul));
}

DWORD Time::GetTotalMiliSeconds()
{
	return (_miliSeconds);
}

void Time::AddMiliSeconds(DWORD MiliSeconds)
{
	_miliSeconds += MiliSeconds;
}

void Time::SetTime(DWORD Miliseconds)
{
	_miliSeconds = Miliseconds;
}