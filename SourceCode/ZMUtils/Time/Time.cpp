#include "Time.h"

LARGE_INTEGER Time::g_ClockFrequency;

Time::Time()
{
	m_TimeElapsed = 0.0;
}

Time::~Time()
{

}