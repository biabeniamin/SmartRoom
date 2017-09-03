#ifndef Room_h
#define Room_h

#include <LanCommunication.h>

typedef enum
{
	CLOSE_DOOR,
	OPEN_DOOR,
	SWITCH_DOOR,
	OPEN_CURTAINS,
	CLOSE_CURTAINS,
	STOP_CURTAINS,
	TURNON_LIGHT,
	TURNOFF_LIGHT,
	SWITCH_LIGHT,

};

class Room
{
public:
	Room(LanCommunication*);
	void SendCommand();
	~Room();
	void TurnOnLight();
	void TurnOffLight();
	void SwitchLight();
	void TurnOnLightOnSeconds(int);
	void OpenCurtains();
	void CloseCurtains();
	void StopCurtains();
	void OpenDoor();
	void CloseDoor();
	void SwitchDoor();

private:
	LanCommunication *_lanComm;
};

#endif // !Room_h