#include "Timer.h"

Timer::Timer()
{
	m_FirstRun = true;
	LARGE_INTEGER b;
	QueryPerformanceCounter(&b);
	
	m_Id = b.LowPart;
}

Timer::~Timer()
{
}

void Timer::Start()
{
	if(m_FirstRun == true)
	{
		QueryPerformanceCounter(&m_Start);
		m_FirstRun = false;
	}

	m_IsActive = true;
}

void Timer::Update()
{
	if(m_IsActive == true)
	{
		QueryPerformanceCounter(&m_End);
		//QueryPerformanceFrequency(&Time::g_ClockFrequency);
		m_TimeSinceStart.Update(m_Start, m_End);
		m_TimeSinceLastFrame.Update(m_OldEnd, m_End);
		m_OldEnd = m_End;
	}
}

void Timer::StartTimeStamp()
{
	QueryPerformanceCounter(&m_TimeStampStart);
}

void Timer::EndTimeStamp()
{
	QueryPerformanceCounter(&m_TimeStampEnd);
	m_TimeStampTime.AppendTime(m_TimeStampStart, m_TimeStampEnd);
}