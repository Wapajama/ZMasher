#ifndef TIMER_H
#define TIMER_H

#include "Time.h"

class Timer
{
public:
	Timer(void);
	~Timer(void);

	void		Update				();
	void		Pause				();
	void		Start				();
	Time		TimeSinceLastFrame	() const;
	Time		TimeSinceStart		() const;
	int			GetId				() const;
	void		SetID				(const int anID);

private:
	Time			myTimeSinceStart;
	Time			myTimeSinceLastFrame;
	LARGE_INTEGER	myStart;
	LARGE_INTEGER	myEnd;
	LARGE_INTEGER	myOldEnd;
	LARGE_INTEGER	myPerformanceFrequency;
	bool			myIsActive;
	bool			myFirstRun;
	int				myId;


//	friend class TimerManager;
};

#endif