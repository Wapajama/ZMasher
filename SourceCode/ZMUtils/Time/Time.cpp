#include "Time.h"


Time::Time(void)
{
	myTimeElapsed = 0.0;
}


Time::~Time(void)
{
}

double Time::GetHours()
{
	return myTimeElapsed / 3600;
}

double Time::GetMinutes()
{
	return myTimeElapsed / 60;
}

double Time::GetSeconds()
{
	return myTimeElapsed;
}

double Time::GetMilliseconds()
{
	return myTimeElapsed * 1000;
}
 
double Time::GetMicroseconds()
{
	return GetMilliseconds() * 1000;
}

void Time::Update(LARGE_INTEGER& aStartCounter, LARGE_INTEGER& aEndCounter, LARGE_INTEGER& aFrequenzy) 
{
	myTimeElapsed = (static_cast<double>(aEndCounter.QuadPart - aStartCounter.QuadPart) / static_cast<double>(aFrequenzy.QuadPart));
}