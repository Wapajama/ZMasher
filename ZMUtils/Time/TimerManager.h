#ifndef TIMERMANAGER_H
#define TIMERMANAGER_H

#include "Timer.h"
#include <vector>

using namespace std;

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

	vector<Timer>		myTimers;
	Timer				myMainTimer;
};

#endif