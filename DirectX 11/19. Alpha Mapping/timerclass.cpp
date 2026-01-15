#include "timerclass.h"


TimerClass::TimerClass()
{
}


TimerClass::TimerClass(const TimerClass& other)
{
}


TimerClass::~TimerClass()
{
}

bool TimerClass::Initialize()
{
	INT64 frequency;
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	if (frequency == 0) return false;
	
	m_frequency = (float)frequency;

	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);
}

void TimerClass::Frame()
{
	INT64 currentTime;
	INT64 elapsedTicks;

	// Query current time.
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	// Calculate the difference in time since the last time we queried for the current time.
	elapsedTicks = currentTime - m_startTime;

	// Calculate the frame time.
	m_frameTime = (float)elapsedTicks / m_frequency;

	// Restart the timer.
	m_startTime = currentTime;

	return;
}

float TimerClass::GetTime()
{
	return m_frameTime;
}