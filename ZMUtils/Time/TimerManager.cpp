#include "TimerManager.h"

TimerManager* TimerManager::myInstance = nullptr;

TimerManager* TimerManager::GetInstance()
{
	if(myInstance == nullptr)
	{
		myInstance = new TimerManager();
	}

	return myInstance;
}

TimerManager::TimerManager(void)
{
	myMainTimer.Start();
}


TimerManager::~TimerManager(void)
{
}

void TimerManager::Update()
{
	myMainTimer.Update();
	for(unsigned int i = 0; i < myTimers.size(); ++i)
	{
		myTimers[i].Update();
	}
}

const Timer& TimerManager::GetMainTimer() const
{
	return myMainTimer;
}

Timer& TimerManager::GetTimer(const int aId)
{
	int id = -1;

	for(unsigned int i = 0; i < myTimers.size(); ++i)
	{
		if(myTimers[i].GetId() == aId)
		{
			id = static_cast<int>(i);
		}
	}

	return myTimers[id];
}

const int TimerManager::CreateTimer()
{
	Timer t;
	t.SetID(myTimers.size());
	myTimers.push_back(t);
	
	return t.GetId();
}

const int TimerManager::CreateAndStartTimer()
{
	Timer t;
	t.Start();
	myTimers.push_back(t);

	return t.GetId();
}