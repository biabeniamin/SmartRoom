#include "stdafx.h"
#include "RgbLedStripPcCase.h"


RgbLedStripPcCase::RgbLedStripPcCase()
{
	Initializing();
}


RgbLedStripPcCase::~RgbLedStripPcCase()
{
}

void RgbLedStripPcCase::Initializing()
{
	TCHAR SdkVer[16];

	hLedApi = LoadLibrary(_T("D:\\Beni\\SmartRoom\\3dRudderController\\GigabyteApiDll\\GLedApi.dll"));

	if (hLedApi == NULL)
	{
		bApiInit = FALSE;
		return;
	}

	pfGetSdkVersion = (PFN_GETSDKVERSION)GetProcAddress(hLedApi, "dllexp_GetSdkVersion");
	pfInitApi = (PFN_INITAPI)GetProcAddress(hLedApi, "dllexp_InitAPI");



	pfRGBPin_Type1 = (PFN_RGBPIN_TYPE1)GetProcAddress(hLedApi, "dllexp_RGBPin_Type1");

	if (pfRGBPin_Type1 == NULL)
	{
		bApiInit = FALSE;
		return;
	}

	pfGetSdkVersion(SdkVer, 16);
	dwLedApiInitOK = pfInitApi();

	if (dwLedApiInitOK == ERROR_INVALID_OPERATION)
	{
		//MessageBox(_T("Initial API fail, all of function call will not work."), _T("Error"), MB_OK | MB_ICONSTOP);
		return;
	}



	if (pfRGBPin_Type1 == NULL)
	{
		bApiInit = FALSE;
		return;
	}

	pfRGBPin_Type1(0, 0, 0);
}

void RgbLedStripPcCase::Notify(Color Color, int interval)
{
	pfRGBPin_Type1(Color.red, Color.blue, Color.green);
	Sleep(1000);
	pfRGBPin_Type1(0, 0, 0);
}
