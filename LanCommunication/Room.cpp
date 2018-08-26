#include "Room.h"


//0 multimedia controller
//1 curtains
//2 room light
//3 door
//4 log
//5 hall light
//6 heater
//8 ac controller

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


void Room::TurnOnHallLight()
{
	_lanComm->SendByte(5, 1);
}
void Room::TurnOffHallLight()
{
	_lanComm->SendByte(5, 0);
}
void Room::SwitchHallLight()
{
	_lanComm->SendByte(5, 2);
}
void Room::TurnOnLightOnSeconds(int ISeconds)
{
	int data[COMMUNICATION_BYTE_COUNT] = {2,_lanComm->GetDeviceAddress(),3,ISeconds};
	_lanComm->SendCommand(data);
}
void Room::TurnOnHallLightOnSeconds(int ISeconds)
{
	int data[COMMUNICATION_BYTE_COUNT] = {5 ,_lanComm->GetDeviceAddress(),3,ISeconds };
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

/*
enum REMOTE_COMMAND_ENUM
{
MEDIA_PLAY = 0,
MEDIA_PAUSE = 1,
MEDIA_PLAYPAUSE = 2,
MEDIA_NEXT = 3,
MEDIA_PREVIOUS = 4,
MEDIA_VOLUMEUP = 5,
MEDIA_VOLUMEDOWN = 6,
MEDIA_VOLUMEMUTE = 7
};
*/
void Room::PlayMultimedia()
{
	_lanComm->SendByte(7,0);
}
void Room::PauseMultimedia()
{
	_lanComm->SendByte(7, 1);
}
void Room::PlayPauseMultimedia()
{
	_lanComm->SendByte(7, 2);
}
void Room::NextMultimedia()
{
	_lanComm->SendByte(7, 3);
}
void Room::PreviousMultimedia()
{
	_lanComm->SendByte(7, 4);
}
void Room::VolumeUpMultimedia()
{
	_lanComm->SendByte(7, 5);
}
void Room::VolumeDownMultimedia()
{
	_lanComm->SendByte(7, 6);
}
void Room::VolumeMuteMultimedia()
{
	_lanComm->SendByte(7, 7);
}
void Room::SpacebarMultimedia()
{
	_lanComm->SendByte(7, 8);
}
void Room::LockMultimedia()
{
	_lanComm->SendByte(7, 9);
}

void Room::SwitchAc()
{
	_lanComm->SendByte(8, 2);

}

void Room::UpdateNixieClockTime()
{
	_lanComm->SendByte(7, 10);
	Serial.println("send new");
}

void Room::Register(DEVICE_TYPE_ENUM type)
{
	int data[COMMUNICATION_BYTE_COUNT] = { 0xA ,_lanComm->GetDeviceAddress(), type, 0xA};
	_lanComm->SendCommand(data);
}