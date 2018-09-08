#include "Arduino.h"
#include "Time.h"
#include "Event.h"

//#define TIMER_INHERITANCE


class Timer
{
public:
	static Timer* GetInstance();


	void SetTimer();

	void TurnOn();
	void TurnOff();
	bool IsTimerOn();

	void Delay(DWORD miliSeconds);

	void TimerEvent();

	DWORD Timer::GetTotalTimerTicks();
	Time* GetUpTime();

	void RegisterEvent(DWORD, void(*)());

protected:
	Timer();
	DWORD _timerTicks;
	Time *_pUpTime;

private:
	static Timer *_instance;

	Event* _events[10];
	BYTE  _eventsCount;


};