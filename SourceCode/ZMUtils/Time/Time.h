#ifndef TIME_H
#define TIME_H

#include <Windows.h>

#define COUNTER_TO_DOUBLE_TIME(start, end) (static_cast<double>(end.QuadPart - start.QuadPart) / static_cast<double>(g_ClockFrequency.QuadPart))

class Time
{
public:
	Time();
	~Time();

	double GetHours();
	double GetMinutes();
	double GetSeconds();
	double GetMilliseconds();
	double GetMicroseconds();
	inline void Update(LARGE_INTEGER& startCounter, LARGE_INTEGER& endCounter){ m_TimeElapsed = COUNTER_TO_DOUBLE_TIME(startCounter, endCounter);}
	inline void AppendTime(LARGE_INTEGER& startCounter, LARGE_INTEGER& endCounter){m_TimeElapsed += COUNTER_TO_DOUBLE_TIME(startCounter, endCounter);}

	static LARGE_INTEGER g_ClockFrequency;
private:
	double	m_TimeElapsed;
};

#endif