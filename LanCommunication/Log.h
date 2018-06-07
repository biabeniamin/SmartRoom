#ifndef LOG_H
#define LOG_H

#include <LanCommunication.h>
#include <DeviceType.h>

class Log
{
public:
	Log(LanCommunication*);
	~Log();

	void DoorUnlocked(int);
	void DoorLocked(int);
	void DoorPinEnteredIncorrectly(int);
	void DoorOpened(int);
	void DoorClosed(int);

	void SendLog(char, int);

private:
	LanCommunication *_lanComm;
};

#endif // LOG_H