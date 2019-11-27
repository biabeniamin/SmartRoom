#pragma once

#include <tchar.h>
#include <windows.h>
#include <winhttp.h>

class HttpClient
{
public:
	HttpClient();
	~HttpClient();

	BYTE DoGetRequest(LPCWSTR PWUrl,
		LPCWSTR PWPage,
		LPWSTR PWBufer,
		DWORD DWMaximumSize);
};

