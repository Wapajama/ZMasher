#ifndef TIMERMANAGER_H
#define TIMERMANAGER_H
#include "DataStructures\GrowArray.h"
#include "Timer.h"


class TimerManager
{
public:
	static TimerManager* GetInstance();

	const Timer&	GetMainTimer		() const;
	const int		CreateTimer			();
	const int		CreateAndStartTimer	();
	Timer&			GetTimer			(const int);
	void			Update				();
private:
	TimerManager								(void);
	~TimerManager								(void);

	static TimerManager* myInstance;

	GrowArray<Timer>		myTimers;
	Timer				myMainTimer;
};

#endif