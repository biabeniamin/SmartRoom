#ifndef Room_h
#define Room_h

#include <LanCommunication.h>
#include <DeviceType.h>

class Room
{
public:
	Room(LanCommunication*);
	~Room();
	void TurnOnLight();
	void TurnOffLight();
	void SwitchLight();
	void TurnOnLightOnSeconds(int);

	void TurnOnHallLight();
	void TurnOffHallLight();
	void SwitchHallLight();
	void TurnOnHallLightOnSeconds(int);

	void OpenCurtains();
	void CloseCurtains();
	void StopCurtains();

	void OpenDoor();
	void CloseDoor();
	void SwitchDoor();

	void TurnOnAudio();
	void TurnOffAudio();
	void SwitchAudio();

	void TurnOnBenchLight();
	void TurnOffBenchLight();
	void SwitchBenchLight();

	void PlayMultimedia();
	void PauseMultimedia();
	void PlayPauseMultimedia();
	void NextMultimedia();
	void PreviousMultimedia();
	void VolumeUpMultimedia();
	void VolumeDownMultimedia();
	void VolumeMuteMultimedia();
	void SpacebarMultimedia();
	void LockMultimedia();

	void SwitchAc();

	void UpdateNixieClockTime();

	void Register(DEVICE_TYPE_ENUM);

private:
	LanCommunication *_lanComm;
};

#endif // !Room_h