#ifndef Room_h
#define Room_h

#include <LanCommunication.h>
/*
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
*/
typedef enum
{
	DOOR_OPEN = 0,
	DOOR_CLOSE = 1,
	PIN_ENTERED_INCORRECTLY = 2,
	LIGHT_OPEN=3,
	LIGHT_CLOSE=4

} LOG_TYPE;
class Room
{
public:
	Room(LanCommunication*);
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
	void PlayMultimedia();
	void PauseMultimedia();
	void NextMultimedia();
	void PreviousMultimedia();
	void VolumeUpMultimedia();
	void VolumeDownMultimedia();
	void LockMultimedia();
	void SendLog(LOG_TYPE, int);

private:
	LanCommunication *_lanComm;
};

#endif // !Room_h