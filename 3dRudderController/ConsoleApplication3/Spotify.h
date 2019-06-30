#pragma once
#include <windows.h>
#include <tlhelp32.h>
#include <vector>
class Spotify
{


public:
	Spotify();
	~Spotify();

	BYTE IsPlaying();
	void GetCurrentTrack(PWCHAR, int);
	BYTE IsAdsPlaying();

private:
	void GetWindowHandle();
	void GetAllWindowsFromProcessID(DWORD DWProcessID);
	BYTE Contanins(PWCHAR Text,
		PCWCHAR Contained);

	HWND _spotifyWindow;
};

