#ifndef TIME_H
#define TIME_H

#include <Windows.h>

#define COUNTER_TO_DOUBLE_TIME(start, end) (static_cast<double>(end.QuadPart - start.QuadPart) / static_cast<double>(g_ClockFrequency.QuadPart))

class Time
{
public:
	Time();
	~Time();

	inline double GetHours() { return m_TimeElapsed / 3600;}
	inline double GetMinutes() { return m_TimeElapsed / 60; }
	inline double GetSeconds() { return m_TimeElapsed; }
	inline double GetMilliseconds() { return m_TimeElapsed * 1000; }
	inline double GetMicroseconds() { return GetMilliseconds() * 1000; }

	inline void Update(LARGE_INTEGER& startCounter, LARGE_INTEGER& endCounter){ m_TimeElapsed = COUNTER_TO_DOUBLE_TIME(startCounter, endCounter);}
	inline void AppendTime(LARGE_INTEGER& startCounter, LARGE_INTEGER& endCounter){ m_TimeElapsed += COUNTER_TO_DOUBLE_TIME(startCounter, endCounter);}

private:
	double m_TimeElapsed;
	static LARGE_INTEGER g_ClockFrequency;
	friend class TimerManager;
};

#endif