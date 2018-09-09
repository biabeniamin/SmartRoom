#include "Timer.h"

Timer* Timer::_instance = 0;

Timer* Timer::GetInstance()
{
	if (!_instance)
	{
		_instance = new Timer();
	}

	return _instance;
}

Timer::Timer()
{
	_pUpTime = new Time();
	_eventsCount = 0;
}

void Timer::SetTimer()
{
	//stop interrupts
	cli();

	// set entire TCCR2A register to 0
	TCCR1A = 0;
	// same for TCCR2B
	TCCR1B = 0;
	//initialize counter value to 0
	TCNT1L = 0;
	// set compare match register for 2khz increments
	// frequency will be 31Hz
	OCR1AL = 156;
	// turn on CTC mode
	TCCR1B |= (1 << WGM12);
	// Set CS02 and CS00 bits for 1024 prescaler
	TCCR1B |= (1 << CS12);
	TCCR1B |= (1 << CS10);

	TurnOff();

	//allow interrupts
	sei();
}

void Timer::TurnOn()
{
	//enable timer compare interrupt
	TIMSK1 |= (1 << OCIE1A);
}

void Timer::TurnOff()
{
	//disable timer compare interrupt
	TIMSK1 &= 0xFF ^ (1 << OCIE1A);

	_timerTicks = 0;
}

bool Timer::IsTimerOn()
{
	return (0 != (TIMSK1 & (1 << OCIE1A)));
}


void Timer::TimerEvent()
{
	//every 32 miliseconds
	_timerTicks++;
	_pUpTime->AddMiliSeconds(10);

	for (BYTE i = 0; i < _eventsCount; i++)
	{
		_events[i]->AddMiliSeconds(-10);
		if (0 == _events[i]->GetTotalMiliSeconds())
		{
			_eventsCount--;
		}
	}
}

DWORD Timer::GetTotalTimerTicks()
{
	return _timerTicks;
}

void Timer::Delay(DWORD miliSeconds)
{
	DWORD startTime;
	
	startTime = GetUpTime()->GetTotalMiliSeconds();

	while ((miliSeconds / 10) > (GetUpTime()->GetTotalMiliSeconds() - startTime))
	{
		/*Serial.print(GetUpTime()->GetTotalMiliSeconds());
		Serial.print(" ");
		Serial.print(GetUpTime()->GetTotalMiliSeconds() - startTime);
		Serial.print(" ");
		Serial.println(startTime);*/
		//digitalWrite(5, digitalRead(5) ^ 1);
		//Serial.print("-");
		///Serial.print("asd:");
		//Serial.print(_timerTicks / 10000);
		//Serial.println(_timerTicks % 10000);
		//wait
	}

}


Time* Timer::GetUpTime()
{
	return _pUpTime;
}

void Timer::RegisterEvent(DWORD interval, void(*callback)())
{
	_events[_eventsCount++] = new Event(interval, callback);
}

#ifndef TIMER_INHERITANCE

ISR(TIMER1_COMPA_vect) { //timer0 interrupt 2kHz toggles pin 8
						 //generates pulse wave of frequency 2kHz/2 = 1kHz (takes two cycles for full wave- toggle high then toggle low)
	Timer::GetInstance()->TimerEvent();
}

#endif // !TIMER_INHERITANCE