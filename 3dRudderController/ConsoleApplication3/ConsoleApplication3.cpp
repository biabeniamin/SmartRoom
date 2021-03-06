// ConsoleApplication3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "3dRudderSDK.h"
#include <tchar.h>
#include <windows.h>
#include <conio.h>
#include "HtttpClient.h"
#include "TcpSocketServer.h"
#include "Keyboard.h"
#include "Spotify.h"
#include "RgbLedStripPcCase.h"
#define HTTP_BUFFER_LENGTH 5000

typedef enum MyEnum
{
	ccBlack = 0,
	ccDarkBlue,
	ccGreen,
	ccGrayBlue,
	ccBrown,
	ccPurple,
	ccKaki,
	ccLightGray,
	ccGray,
	ccBlue,
	ccFluorescentGreen,
	ccTurquoise,
	ccRed,
	ccFluorescentPink,
	ccFluorescentYellow,
	ccWhite
}ConsoleColor;

void RemoteCommandReceived(REMOTE_COMMAND Message, BYTE, BYTE);

TcpSocketServer tcpSocket(10000, RemoteCommandReceived);

Keyboard keyboard;
Spotify spotify;
RgbLedStripPcCase _rgbPcCase;



void color(ConsoleColor t, ConsoleColor f)
{
	HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(H, WORD(f) * 16 + WORD(t));
}


class CEvent : public ns3dRudder::IEvent
{
	void OnConnect(uint32_t nDeviceNumber)
	{
		printf("\tOnConnect( %d )\n", nDeviceNumber);
	}
	void OnDisconnect(uint32_t nDeviceNumber)
	{
		printf("\tOnDisconnect(%d)\n", nDeviceNumber);
	}
};



void RemoteCommandReceived(REMOTE_COMMAND Message, BYTE Argument1, BYTE Argument2)
{
	Color color;

	color = { 0, 0, 0 };

	printf("Message received from remote using tcp socket %d %d %d\n", Message, Argument1, Argument2);

	switch (Message)
	{
	case MEDIA_PLAYPAUSE:
		keyboard.PlayPause();
		break;
	case MEDIA_NEXT:
		keyboard.Next();
		break;
	case MEDIA_PREVIOUS:
		keyboard.Previous();
		break;
	case MEDIA_VOLUMEUP:
		for (int i = 0; i < Argument1; i++)
		{
			keyboard.VolumeUp();
		}
		break;
	case MEDIA_VOLUMEDOWN:
		for (int i = 0; i < Argument1; i++)
		{
			keyboard.VolumeDown();
		}
		break;
	case MEDIA_VOLUMEMUTE:
		keyboard.VolumeMute();
		break;
	case SPACEBAR:
		keyboard.Spacebar();
		break;
	case LOCK:
		keyboard.Lock();
		break;
	case STOP_MUSIC:
		if (spotify.IsPlaying())
		{
			keyboard.PlayPause();
		}
		break;
	case ALERT:
		color.red = (Argument2 >> 2) & 0b1;
		color.green = (Argument2 >> 1) & 0b1;
		color.blue = (Argument2 >> 0) & 0b1;
		_rgbPcCase.Notify(color, Argument1);
		break;
	default:

		break;
	}

}
DWORD WINAPI TcpSocketServerProc(__in LPVOID lpParameter)
{
	tcpSocket.Start();

	return 0;
}

DWORD WINAPI SpotifyAdsBlockerProc(__in LPVOID lpParameter)
{
	while (1)
	{
		try
		{
			if (spotify.IsAdsPlaying())
			{
				spotify.Mute();

				while (spotify.IsAdsPlaying())
				{
					Sleep(500);
				}

				spotify.Unmute();
			}

			Sleep(500);
		}
		catch (const char *msg)
		{
			printf("error %s\n", msg);
		}
	}

	return 0;
}

void StartTcpSocketServer()
{
	CreateThread(NULL,
		0,
		TcpSocketServerProc,
		0,
		0,
		NULL);
}

void StartSpotifyAdsBlocker()
{
	CreateThread(NULL,
		0,
		SpotifyAdsBlockerProc,
		0,
		0,
		NULL);
}


LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		//...       
	case WM_QUERYENDSESSION: return TRUE;
	case WM_ENDSESSION:
		if (wParam) PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

int main()
{
	try
	{
	WCHAR pwBuffer[HTTP_BUFFER_LENGTH];
	HttpClient hcClient;
	float fLastRotation;
	float fLastPitch;
	BYTE bPitchActivated;
	BYTE bRollActivated;
	BYTE bCurtainsMode;

	SYSTEMTIME stLastRightClick;
	SYSTEMTIME stLastLeftClick;
	SYSTEMTIME stCurrent;

	fLastRotation = 0;
	fLastPitch = 0;
	bPitchActivated = 0;
	bRollActivated = 0;
	bCurtainsMode = 0;

	GetSystemTime(&stLastLeftClick);
	GetSystemTime(&stLastRightClick);

	
	StartTcpSocketServer();
	StartSpotifyAdsBlocker();

	CEvent m_Event;
	ns3dRudder::CSdk* pSdk = ns3dRudder::GetSDK();
	pSdk->SetEvent(&m_Event);
	pSdk->Init();


	color(ccFluorescentPink, ccBlack);
	uint16_t nVersion = pSdk->GetVersion(0);
	printf("Firmware Version %04x\n", nVersion);
	uint16_t nSdkVersion = pSdk->GetSDKVersion();
	printf("SDK Version %04x\n", nSdkVersion);
	color(ccWhite, ccBlack);

	color(ccRed, ccBlack);
	printf("Press 'Q' to quit\n");
	color(ccFluorescentYellow, ccBlack);
	printf("Press '+' and '-' to change mode\n");
	BOOL bFirst = TRUE;
	ns3dRudder::ModeAxis nMode = ns3dRudder::UserRefAngle;

	//start audio system
	hcClient.DoGetRequest(L"192.168.0.108",
		L"/index.php?cmd=audioOn",
		pwBuffer,
		HTTP_BUFFER_LENGTH);


	while (1)
	{
		ns3dRudder::Axis lAxis;
		ns3dRudder::Axis lAxisUserOffset;
		ns3dRudder::CurveArray  localCurve;

		ns3dRudder::Status status = pSdk->GetStatus(0);
		switch (status)
		{
		case ns3dRudder::NoFootStayStill:
			printf("Status : Don't put your Feet !!! Stay still 5s\r");
			bFirst = TRUE;
			break;
		case ns3dRudder::Initialisation:
			printf("Status : Initialisation                       \r");
			bFirst = TRUE;
			break;
		case ns3dRudder::PutYourFeet:
			printf("Status : Please put your feet                 \r");
			bFirst = TRUE;
			break;
		case ns3dRudder::PutSecondFoot:
			printf("Status :  Put your second foot                \r");
			bFirst = TRUE;
			break;
		case ns3dRudder::StayStill:
			printf("Status : Stay still                           \r");

			bCurtainsMode = 0;

			pSdk->GetUserOffset(0, &lAxisUserOffset);
			color(ccFluorescentGreen, ccBlack);
			if (bFirst)
			{
				printf("GetUserOffset : ");
				color(ccWhite, ccBlack);
				printf("aX: %2.5f aY %2.5f aZ %2.5f rZ %2.5f                                         \n ", lAxisUserOffset.m_aX, lAxisUserOffset.m_aY, lAxisUserOffset.m_aZ, lAxisUserOffset.m_rZ);
				bFirst = FALSE;
			}
			break;
		case ns3dRudder::InUse:
			printf("Status : 3DRudder in use                      \r");
			bFirst = TRUE;
			break;
		case ns3dRudder::ExtendedMode:
			printf("Status : Extended functions activated         \r");
			bFirst = TRUE;
			break;
		}

		switch (status)
		{
		case ns3dRudder::InUse:
		case ns3dRudder::ExtendedMode:
			break;
		default:
			Sleep(100);
			continue;
		}

		
		if (pSdk->GetAxis(0, nMode, &lAxis, &localCurve) != ns3dRudder::NotReady)
		{
			printf("Axes : ");
			printf("aX: %2.5f aY %2.5f aZ %2.5f rZ %2.5f ", lAxis.m_aX, lAxis.m_aY, lAxis.m_aZ, lAxis.m_rZ);

			

			//yaw
			if ((fLastRotation > lAxis.GetPhysicalYaw() + 2
				&& 0 > lAxis.GetPhysicalYaw())
				|| (-40 > lAxis.GetPhysicalYaw()))
			{
				keyboard.VolumeDown();

				fLastRotation = lAxis.GetPhysicalYaw();
				continue;
			}
			else if ((fLastRotation < lAxis.GetPhysicalYaw() - 2
				&& 0 < lAxis.GetPhysicalYaw())
				|| (40 < lAxis.GetPhysicalYaw()))
			{
				keyboard.VolumeUp();

				fLastRotation = lAxis.GetPhysicalYaw();
				continue;
			}


			if (10 < lAxis.GetPhysicalPitch() &&
				0 == bPitchActivated
				&& 0.3 > lAxis.GetZAxis())
			{
				bCurtainsMode ^= 0x1;
				if (1 == bCurtainsMode)
				{
					pSdk->PlaySnd(0,
						1000,
						500);
				}
				else
				{
					pSdk->PlaySnd(0,
						100,
						500);
				}

				bPitchActivated = 1;
				continue;
			}
			else if (-10 > lAxis.GetPhysicalPitch() &&
				0 == bPitchActivated)
			{
				if (1 == bCurtainsMode)
				{
					hcClient.DoGetRequest(L"192.168.0.108",
						L"/index.php?cmd=stopCurtains",
						pwBuffer,
						HTTP_BUFFER_LENGTH);
				}
				else
				{
					keyboard.PlayPause();
				}

				bPitchActivated = 1;
			}
			else if (-2 < lAxis.GetPhysicalPitch() &&
				2 > lAxis.GetPhysicalPitch() &&
				1 == bPitchActivated)
			{
				bPitchActivated = 0;
			}


			//track control
			if (10 < lAxis.GetPhysicalRoll() &&
				0 == bRollActivated)
			{
				if (1 == bCurtainsMode)
				{
					hcClient.DoGetRequest(L"192.168.0.108",
						L"/index.php?cmd=openCurtains",
						pwBuffer,
						HTTP_BUFFER_LENGTH);
				}
				else
				{
					keyboard.Next();
				}

				bRollActivated = 1;
				continue;
			}
			else if (-10 > lAxis.GetPhysicalRoll() &&
				0 == bRollActivated)
			{
				if (1 == bCurtainsMode)
				{
					hcClient.DoGetRequest(L"192.168.0.108",
						L"/index.php?cmd=closeCurtains",
						pwBuffer,
						HTTP_BUFFER_LENGTH);
				}
				else
				{
					keyboard.Previous();
				}

				bRollActivated = 1;
				continue;
			}
			else if (-2 < lAxis.GetPhysicalRoll() &&
				2 > lAxis.GetPhysicalRoll() &&
				1 == bRollActivated)
			{
				bRollActivated = 0;
			}

			if (0.7 < lAxis.GetZAxis())
			{

				GetSystemTime(&stCurrent);
				if (((2 < stCurrent.wSecond - stLastLeftClick.wSecond)
					|| (-2 > stCurrent.wSecond - stLastLeftClick.wSecond))
					&& 5 < lAxis.GetPhysicalPitch())
				{
					if (1 == bCurtainsMode)
					{
						hcClient.DoGetRequest(L"192.168.0.108",
							L"/index.php?cmd=benchLightSwitch",
							pwBuffer,
							HTTP_BUFFER_LENGTH);
					}
					else
					{
						hcClient.DoGetRequest(L"192.168.0.108",
							L"/index.php?cmd=lightSwitch",
							pwBuffer,
							HTTP_BUFFER_LENGTH);
					}
					stLastLeftClick = stCurrent;
				}
			}
			else if (-0.7 > lAxis.GetZAxis())
			{
				GetSystemTime(&stCurrent);
				if (((2 < stCurrent.wSecond - stLastRightClick.wSecond)
					|| (-2 > stCurrent.wSecond - stLastRightClick.wSecond))
					&& 5 < lAxis.GetPhysicalPitch())
				{
					if (1 == bCurtainsMode)
					{
						hcClient.DoGetRequest(L"192.168.0.108",
							L"/index.php?cmd=audioSwitch",
							pwBuffer,
							HTTP_BUFFER_LENGTH);
					}
					else
					{
						hcClient.DoGetRequest(L"192.168.0.108",
							L"/index.php?cmd=switchDoor",
							pwBuffer,
							HTTP_BUFFER_LENGTH);
					}
					stLastRightClick = stCurrent;
				}

			}


			
		}
		Sleep(15);
	}
	}
	catch (const char *msg)
	{
		printf("error %s\n", msg);
	}
    return 0;
}

