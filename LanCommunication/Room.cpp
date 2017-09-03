#include "Room.h"
Room::Room(LanCommunication *lanCom)
{
	_lanComm = lanCom;
}

Room::~Room()
{
}
void Room::TurnOnLight()
{
	_lanComm->SendByte(2, 1);
}
void Room::TurnOffLight()
{
	_lanComm->SendByte(2, 0);
}
void Room::SwitchLight()
{
	_lanComm->SendByte(2, 2);
}
void Room::TurnOnLightOnSeconds(int ISeconds)
{
	int data[COMMUNICATION_BYTE_COUNT] = {2,3,ISeconds,1};
	_lanComm->SendCommand(data);
}
void Room::OpenCurtains()
{
	_lanComm->SendByte(1, 1);
}
void Room::CloseCurtains()
{
	_lanComm->SendByte(1, 0);
}
void Room::StopCurtains()
{
	_lanComm->SendByte(1, 2);
}
void Room::OpenDoor()
{
	_lanComm->SendByte(3, 1);
}
void Room::CloseDoor()
{
	_lanComm->SendByte(3, 0);
}
void Room::SwitchDoor()
{
	_lanComm->SendByte(3, 2);
}