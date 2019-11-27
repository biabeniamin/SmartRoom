#pragma once

#include <windows.h>


class Keyboard
{
public:
	Keyboard();
	~Keyboard();

	void VolumeUp();
	void VolumeDown();
	void VolumeMute();

	void Next();
	void Previous();

	void PlayPause();
	void Play();
	void Pause();

	void Spacebar();
	void Lock();

private:
	void PressKey(WORD WKey);
	void PressTwoKeys(WORD WKey1, WORD WKey2);
};

