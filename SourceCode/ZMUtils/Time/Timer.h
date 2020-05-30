#ifndef TIMER_H
#define TIMER_H

#include "Time.h"

class Timer
{
public:
	Timer(void);
	~Timer();

	void Update();
	inline void Pause() {m_IsActive = false;}
	void Start();
	inline Time TimeSinceLastFrame() const { return m_TimeSinceLastFrame; }
	inline Time TimeSinceStart() const { return m_TimeSinceStart; } 
	inline int GetId() const { return m_Id; }
	inline void SetID(const int id) { m_Id = id; }

	// Will append whatever time it took 
	void StartTimeStamp();
	void EndTimeStamp();

	inline Time GetTotalTimeStampTime() { return m_TimeStampTime; };

private:
	Time m_TimeSinceStart;
	Time m_TimeSinceLastFrame;
	Time m_TimeStampTime;
	LARGE_INTEGER m_Start;
	LARGE_INTEGER m_End;
	LARGE_INTEGER m_OldEnd;

	LARGE_INTEGER m_TimeStampStart;
	LARGE_INTEGER m_TimeStampEnd;
	bool m_IsActive;
	bool m_FirstRun;
	int	m_Id;
};

#endif