#pragma once

#include<Windows.h>
#include "Resource.h"

struct Color
{
	int red;
	int green;
	int blue;
};

class RgbLedStripPcCase
{
public:
	RgbLedStripPcCase();
	~RgbLedStripPcCase();

	void Notify(struct Color, int interval);

private:
	void Initializing();

	typedef DWORD(*PFN_GETSDKVERSION)(LPTSTR lpReturnBuf, int bufLen);
	typedef DWORD(*PFN_INITAPI)();

	typedef DWORD(*PFN_RGBPIN_TYPE1)(int pin1, int pin2, int pin3);

	PFN_GETSDKVERSION pfGetSdkVersion = NULL;
	PFN_INITAPI pfInitApi = NULL;

	PFN_RGBPIN_TYPE1 pfRGBPin_Type1 = NULL;

	HINSTANCE hLedApi = NULL;
	BOOL bApiInit = TRUE;

	DWORD dwLedApiInitOK = ERROR_SUCCESS;
};

