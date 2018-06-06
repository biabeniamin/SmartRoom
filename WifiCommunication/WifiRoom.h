#ifndef WIFIROOM_H
#define WIFIROOM_H

#include <WifiCommunication.h>

typedef enum
{
	DOOR_OPEN = 0,
	DOOR_CLOSE = 1,
	PIN_ENTERED_INCORRECTLY = 2,
	LIGHT_OPEN=3,
	LIGHT_CLOSE=4

} LOG_TYPE;
class WifiRoom
{
public:
	WifiRoom();
	~WifiRoom();
	void MotionDetectedStairs();
	void TurnOffLight();

private:
	//LanCommunication *_lanComm;
	WifiCommunication * _wifiComm;
};

#endif // !WIFIROOM_H