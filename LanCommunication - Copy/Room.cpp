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
	_lanComm->SendOlderVersionByte(2, 1);
}
void Room::TurnOffLight()
{
	_lanComm->SendOlderVersionByte(2, 0);
}
void Room::SwitchLight()
{
	_lanComm->SendOlderVersionByte(2, 2);
}
void Room::TurnOnLightOnSeconds(int ISeconds)
{
	int data[COMMUNICATION_BYTE_COUNT] = {2,3,ISeconds,1};
	_lanComm->SendCommand(data);
}
void Room::OpenCurtains()
{
	_lanComm->SendOlderVersionByte(1, 1);
}
void Room::CloseCurtains()
{
	_lanComm->SendOlderVersionByte(1, 0);
}
void Room::StopCurtains()
{
	_lanComm->SendOlderVersionByte(1, 2);
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
void Room::PlayMultimedia()
{
	_lanComm->SendOlderVersionByte(0,1);
}
void Room::PauseMultimedia()
{
	_lanComm->SendOlderVersionByte(0, 2);
}
void Room::NextMultimedia()
{
	_lanComm->SendOlderVersionByte(0, 3);
}
void Room::PreviousMultimedia()
{
	_lanComm->SendOlderVersionByte(0, 4);
}
void Room::VolumeUpMultimedia()
{
	_lanComm->SendOlderVersionByte(0, 5);
}
void Room::VolumeDownMultimedia()
{
	_lanComm->SendOlderVersionByte(0, 6);
}
void Room::LockMultimedia()
{
	_lanComm->SendOlderVersionByte(0, 7);
}
void Room::SendLog(LOG_TYPE value, int source)
{
	int data[COMMUNICATION_BYTE_COUNT] = { 4,_lanComm->GetDeviceAddress(), value, source };
	_lanComm->SendCommand(data);
}