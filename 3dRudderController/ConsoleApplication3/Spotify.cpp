#include "stdafx.h"
#include "Spotify.h"


Spotify::Spotify()
{
	GetWindowHandle();
}


Spotify::~Spotify()
{
}

BYTE Spotify::Contanins(PWCHAR Text,
	PCWCHAR Contained)
{
	for (DWORD i = 0; i < _tcslen(Text); i++)
	{
		DWORD same;

		same = 0;

		for (DWORD j = 0; j < _tcslen(Contained); j++)
		{
			if (Text[i + j] == Contained[j])
			{
				same++;
			}
			else
			{
				break;
			}
		}

		if (same == _tcslen(Contained))
		{
			return 1;
		}
	}

	return 0;
}

void Spotify::GetAllWindowsFromProcessID(DWORD DWProcessID)
{
	// find all hWnds (vhWnds) associated with a process id (dwProcessID)
	HWND hCurWnd;
	wchar_t text[500];
	std::vector<const wchar_t*> exceptions;

	hCurWnd = NULL;

	exceptions.push_back(L"MSCTFIME UI");
	exceptions.push_back(L"Default IME");
	exceptions.push_back(L"Default IME");

	do
	{
		hCurWnd = FindWindowEx(NULL, hCurWnd, NULL, NULL);
		DWORD dwProcessID = 0;
		GetWindowThreadProcessId(hCurWnd, &dwProcessID);
		if (dwProcessID == DWProcessID)
		{
			BYTE isException;

			isException = 0;

			GetWindowText(hCurWnd, text, 500);
			if (2 > _tcslen(text))
			{
				continue;
			}

			for (size_t i = 0; i < exceptions.size(); i++)
			{
				if (Contanins(text, exceptions[i]))
				{
					isException = 1;
					break;
				}
			}
			if (1 == isException)
			{
				continue;
			}

			wprintf(L"Found hWnd %d %d\n", hCurWnd, dwProcessID);
			_tprintf(TEXT("--+-- %s \n"), text);

			_spotifyWindow = hCurWnd;
			break;
		}
	} while (hCurWnd != NULL);
}

void Spotify::GetWindowHandle()
{
		PROCESSENTRY32 process;
		HANDLE hh;
		std::vector<PROCESSENTRY32> spotifyProcesses;

		hh = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hh == INVALID_HANDLE_VALUE)
		{
			return;
		}

		process.dwSize = sizeof(PROCESSENTRY32);
		Process32First(hh,
			&process);

		//add all spotify processes
		while (Process32Next(hh, &process))
		{
			if (Contanins(process.szExeFile, TEXT("Spotify")))
			{
				_tprintf(TEXT("%s %d %d\n"), process.szExeFile, process.th32ProcessID, process.th32ParentProcessID);
				spotifyProcesses.push_back(process);

			}
		}

		if (0 == spotifyProcesses.size())
		{
			_spotifyWindow = NULL;
			return;
		}

		//substract child processes
		for (int i = 0; i < spotifyProcesses.size(); i++)
		{
			BYTE contains;

			contains = 0;

			for (int j = 0; j < spotifyProcesses.size(); j++)
			{
				if (spotifyProcesses[j].th32ProcessID == spotifyProcesses[i].th32ParentProcessID)
				{
					contains = 1;
					break;
				}
			}

			if (1 == contains)
			{
				spotifyProcesses.erase(spotifyProcesses.begin() + i);
				i--;
			}
		}

		_tprintf(L"Substracted processes \n");
		for (int i = 0; i < spotifyProcesses.size(); i++)
		{
			process = spotifyProcesses[i];
			_tprintf(TEXT("%s %d %d\n"), process.szExeFile, process.th32ProcessID, process.th32ParentProcessID);
		}

		process = spotifyProcesses[0];

		GetAllWindowsFromProcessID(process.th32ProcessID);
}

BYTE Spotify::IsPlaying()
{
	wchar_t text[500];

	GetCurrentTrack(text, 500);

	if (0 == wcscmp(text, L"Spotify"))
	{
		return 0;
	}

	return 1;
}

void Spotify::GetCurrentTrack(PWCHAR PCurrentTrack, int MaxLenght)
{
	if (NULL == _spotifyWindow)
	{
		GetWindowHandle();

		if (NULL == _spotifyWindow)
		{
			return;
		}
	}

	GetWindowText(_spotifyWindow, PCurrentTrack, MaxLenght);
}

BYTE Spotify::IsAdsPlaying()
{
	wchar_t text[500];

	GetCurrentTrack(text, 500);

	if (Contanins(text, L"Afla mai multe"))
	{
		return 1;
	}
	else if (Contanins(text, L"Pureboost Go"))
	{
		return 1;
	}
	else if (Contanins(text, L"Advertisement"))
	{
		return 1;
	}
	/*else if (Contanins(text, L"Spotify"))
	{
		return 1;
	}*/

	return 0;
}
