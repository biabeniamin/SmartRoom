#pragma once

#define COMMUNICATION_BYTE_COUNT 5
#define VERIFICATION_BYTE_COUNT 4
#define ASCII_VALUES 0
#include "Arduino.h"

class LanCommunication
{
public:
	LanCommunication(int, int, void (*)(int),int (*)(),int (*)());
	~LanCommunication();
	void SendCommand(int bytes[COMMUNICATION_BYTE_COUNT]);
	void SendByte(int,int);
	void SendOlderVersionByte(int, int);
	int IsCommandAvailable();
	int ReadCommand();
	int *GetLastCommand();
	int GetDeviceAddress();
private:
	//functions
	void _writeVerificationBytes();
	void _writeByte(int value);
	int _readByte();
	int _readCount();
	void _activateMax();
	void _deactivateMax();
	int _checkVerificationBytes();
	//variables
	int _address;
	int _bytesInLastCommand[COMMUNICATION_BYTE_COUNT];
	int _triggerPin;
	void (*_writeFunct)(int);
	int(*_readFunct)();
	int(*_countFunct)();
};

