#ifndef TIMERMANAGER_H
#define TIMERMANAGER_H
#include "DataStructures\GrowArray.h"
#include "Timer.h"


class TimerManager
{
public:
	static TimerManager* GetInstance();
	static void Create(TimerManager* instance);
	inline Timer& GetMainTimer() { return m_MainTimer; }
	const int CreateTimer();
	const int CreateAndStartTimer();
	Timer& GetTimer(const int);
	void Update();

private:
	TimerManager(void);
	~TimerManager(void);

	static TimerManager* m_Instance;

	GrowArray<Timer> m_Timers;
	Timer m_MainTimer;
};

#endif