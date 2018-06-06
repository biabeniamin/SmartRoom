#include "WifiRoom.h"
WifiRoom::WifiRoom()
{
	_wifiComm = new WifiCommunication();
}

WifiRoom::~WifiRoom()
{
}
void WifiRoom::MotionDetectedStairs()
{
	_wifiComm->SendCommand("asd");
}
void WifiRoom::TurnOffLight()
{
}