#include "Timer.h"


Timer::Timer(void)
{
	myFirstRun = true;
	LARGE_INTEGER b;
	QueryPerformanceCounter(&b);

	myId = b.LowPart;
}


Timer::~Timer(void)
{
}

void Timer::Start()
{
	if(myFirstRun == true)
	{
		QueryPerformanceCounter(&myStart);
		myFirstRun = false;
	}

	myIsActive = true;
}

void Timer::Pause()
{
		myIsActive = false;
}

void Timer::Update()
{
	if(myIsActive == true)
	{
		QueryPerformanceFrequency(&myPerformanceFrequency);
		QueryPerformanceCounter(&myEnd);

		myTimeSinceStart.Update(myStart, myEnd, myPerformanceFrequency);
		myTimeSinceLastFrame.Update(myOldEnd, myEnd, myPerformanceFrequency);
		myOldEnd = myEnd;
	}
}

Time Timer::TimeSinceStart() const
{
	return myTimeSinceStart;
}

Time Timer::TimeSinceLastFrame() const
{
	return myTimeSinceLastFrame;
}

int Timer::GetId() const
{
	return myId;
}

void Timer::SetID(const int anID)
{
	myId = anID;
}