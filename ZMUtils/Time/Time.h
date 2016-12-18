#ifndef TIME_H
#define TIME_H

#include <Windows.h>

class Time
{
public:
	Time(void);
	~Time(void);

	double	GetHours		();
	double	GetMinutes		();
	double	GetSeconds		();
	double	GetMilliseconds	();
	double	GetMicroseconds	();
	void	Update	(LARGE_INTEGER& aStartCounter, LARGE_INTEGER& aEndCounter, LARGE_INTEGER& aFrequenzy);

private:
	double	myTimeElapsed;
};

#endif