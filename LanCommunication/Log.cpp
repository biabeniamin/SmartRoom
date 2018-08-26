#include "Log.h"

#define DOOR_UNLOCK_LOG 0
#define DOOR_LOCK_LOG 1
#define PIN_ENTERED_INCORRECTLY_LOG 2
#define DOOR_OPEN_LOG 3
#define DOOR_CLOSE_LOG 4

#define LIGHT_OPEN_LOG 1
#define LIGHT_CLOSE_LOG 0

#define AC_SWITCH_LOG 0

Log::Log(LanCommunication *lanCom)
{
	_lanComm = lanCom;
}

Log::~Log()
{
}

void Log::SendLog(char value, int source)
{
	int data[COMMUNICATION_BYTE_COUNT] = { 4,_lanComm->GetDeviceAddress(), value, source };
	_lanComm->SendCommand(data);
}

void Log::DoorUnlocked(int source)
{
	SendLog(DOOR_UNLOCK_LOG, source);
}

void Log::DoorLocked(int source)
{
	SendLog(DOOR_LOCK_LOG, source);
}

void Log::DoorPinEnteredIncorrectly(int source)
{
	SendLog(PIN_ENTERED_INCORRECTLY_LOG, source);
}

void Log::DoorOpened(int source)
{
	SendLog(DOOR_OPEN_LOG, source);
}

void Log::DoorClosed(int source)
{
	SendLog(DOOR_CLOSE_LOG, source);
}

void Log::LightOpen(int source)
{
	SendLog(LIGHT_OPEN_LOG, source);
}

void Log::LightClose(int source)
{
	SendLog(LIGHT_CLOSE_LOG, source);
}

void Log::AcSwitch(int source)
{
	SendLog(AC_SWITCH_LOG, source);
}