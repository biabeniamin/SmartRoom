#include "stdafx.h"
#include "Keyboard.h"


Keyboard::Keyboard()
{
}


Keyboard::~Keyboard()
{
}

void Keyboard::VolumeUp()
{

	PressKey(VK_VOLUME_UP);
}

void Keyboard::VolumeDown()
{

	PressKey(VK_VOLUME_DOWN);
}
void Keyboard::VolumeMute()
{

	PressKey(VK_VOLUME_MUTE);
}

void Keyboard::Next()
{
	PressKey(VK_MEDIA_NEXT_TRACK);
}

void Keyboard::Previous()
{
	PressKey(VK_MEDIA_PREV_TRACK);
}

void Keyboard::PlayPause()
{
	PressKey(VK_MEDIA_PLAY_PAUSE);
}

void Keyboard::Play()
{
	PressKey(VK_MEDIA_PLAY_PAUSE);
}

void Keyboard::Pause()
{
	PressKey(VK_PAUSE);
}

void Keyboard::Spacebar()
{
	PressKey(VK_SPACE);
}

void Keyboard::Lock()
{
	LockWorkStation();
}

void Keyboard::PressKey(WORD WKey)
{
	// This structure will be used to create the keyboard
	// input event.
	INPUT ip;

	// Set up a generic keyboard event.
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0; // hardware scan code for key
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	// Press the  key
	ip.ki.wVk = WKey; // virtual-key code for the "a" key
	ip.ki.dwFlags = 0; // 0 for key press
	SendInput(1, &ip, sizeof(INPUT));

	// Release the "A" key
	ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
	SendInput(1, &ip, sizeof(INPUT));
}

void Keyboard::PressTwoKeys(WORD WKey1, WORD WKey2)
{
	// This structure will be used to create the keyboard
	// input event.
	INPUT ip[2];

	// Set up a generic keyboard event.
	ip[0].type = INPUT_KEYBOARD;
	ip[0].ki.wScan = 0; // hardware scan code for key
	ip[0].ki.time = 0;
	ip[0].ki.dwExtraInfo = 0;

	ip[1].type = INPUT_KEYBOARD;
	ip[1].ki.wScan = 0; // hardware scan code for key
	ip[1].ki.time = 0;
	ip[1].ki.dwExtraInfo = 0;

	// Press the  key
	ip[0].ki.wVk = WKey1; // virtual-key code for the "a" key
	ip[0].ki.dwFlags = 0; // 0 for key press
	ip[1].ki.wVk = WKey2; // virtual-key code for the "a" key
	ip[1].ki.dwFlags = 0; // 0 for key press

	SendInput(1, ip, sizeof(INPUT));
	SendInput(1, &ip[1], sizeof(INPUT));

	// Release the "A" key
	ip[0].ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
	ip[1].ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
	SendInput(2, ip, sizeof(INPUT));
}