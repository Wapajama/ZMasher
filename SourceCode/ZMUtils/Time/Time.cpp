#include "Time.h"

LARGE_INTEGER Time::g_ClockFrequency;

Time::Time()
{
	m_TimeElapsed = 0.0;
	QueryPerformanceFrequency(&g_ClockFrequency);
}


Time::~Time()
{
}

double Time::GetHours()
{
	return m_TimeElapsed / 3600;
}

double Time::GetMinutes()
{
	return m_TimeElapsed / 60;
}

double Time::GetSeconds()
{
	return m_TimeElapsed;
}

double Time::GetMilliseconds()
{
	return m_TimeElapsed * 1000;
}
 
double Time::GetMicroseconds()
{
	return GetMilliseconds() * 1000;
}