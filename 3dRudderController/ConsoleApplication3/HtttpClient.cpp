#include "stdafx.h"
#include "HtttpClient.h"


HttpClient::HttpClient()
{
}


HttpClient::~HttpClient()
{
}

BYTE HttpClient::DoGetRequest(LPCWSTR PWUrl,
	LPCWSTR PWPage,
	LPWSTR PWBufer,
	DWORD DWMaximumSize)
{
	BOOL  bResults = FALSE;
	HINTERNET hSession = NULL,
		hConnect = NULL,
		hRequest = NULL;

	// Use WinHttpOpen to obtain a session handle.
	hSession = WinHttpOpen(L"A WinHTTP Example Program/1.0",
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS, 0);

	// Specify an HTTP server.
	if (hSession)
		hConnect = WinHttpConnect(hSession, PWUrl,
			INTERNET_DEFAULT_HTTP_PORT, 0);

	// Create an HTTP Request handle.
	if (hConnect)
		hRequest = WinHttpOpenRequest(hConnect, L"GET",
			PWPage,
			NULL, WINHTTP_NO_REFERER,
			WINHTTP_DEFAULT_ACCEPT_TYPES,
			0);

	// Send a Request.
	if (hRequest)
		bResults = WinHttpSendRequest(hRequest,
			WINHTTP_NO_ADDITIONAL_HEADERS,
			0, WINHTTP_NO_REQUEST_DATA, 0,
			0, 0);

	// PLACE ADDITIONAL CODE HERE.

	// Report any errors.
	if (!bResults)
		printf("Error %d has occurred.\n", GetLastError());

	if (bResults)
		bResults = WinHttpReceiveResponse(hRequest, NULL);

	if (bResults)
	{
		DWORD dwSize;
		do
		{
			DWORD dwDownloaded;

			// Check for available data.
			dwSize = 0;
			if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
			{
				printf("Error %u in WinHttpQueryDataAvailable.\n",
					GetLastError());
				break;
			}

			// No more available data.
			if (0 == dwSize)
				break;

			if (DWMaximumSize < dwSize)
			{
				break;
			}

			// Allocate space for the buffer.
			if (!PWBufer)
			{
				printf("Out of memory\n");
				break;
			}

			memset(PWBufer,
				0,
				sizeof(WCHAR) * DWMaximumSize);
			if (!WinHttpReadData(hRequest, (LPVOID)PWBufer,
				dwSize, &dwDownloaded))
			{
				printf("Error %u in WinHttpReadData.\n", GetLastError());
			}
			else
			{
				printf("%s", PWBufer);
			}

			// This condition should never be reached since WinHttpQueryDataAvailable
			// reported that there are bits to read.
			if (!dwDownloaded)
				break;

		} while (dwSize > 0);

	}
	
	return bResults;

	// Close any open handles.
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);
}
