#pragma once
#include <windows.h>
#include <tlhelp32.h>
#include <vector>

#include <initguid.h>
#include <math.h>
#include <appmodel.h>
#include <cguid.h>
#include <atlbase.h>
#include <stdio.h>
#include <mmdeviceapi.h>
#include <audiopolicy.h>
#include <endpointvolume.h>
#include <functiondiscoverykeys_devpkey.h>

#define AUDCLNT_S_NO_SINGLE_PROCESS AUDCLNT_SUCCESS (0x00d)

#define LOG(format, ...) wprintf(format L"\n", __VA_ARGS__)

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
	HRESULT GetAudioSessionEnumerator(IAudioSessionEnumerator **ppAudioSessionEnumerator);
	HRESULT GetSpotifyAudioSession();

	HWND _spotifyWindow;
};

