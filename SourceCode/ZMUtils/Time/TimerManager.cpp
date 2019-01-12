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

void TimerManager::Create(TimerManager* instance)
{
	if (instance)
	{
		myInstance = instance;
		return;
	}
	myInstance = new TimerManager();
}

TimerManager::TimerManager(void):myTimers(16)
{
	myMainTimer.Start();
}

TimerManager::~TimerManager(void)
{
}

void TimerManager::Update()
{
	myMainTimer.Update();
	for(unsigned int i = 0; i < myTimers.Size(); ++i)
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

	for(int i = 0; i < myTimers.Size(); ++i)
	{
		if(myTimers[i].GetId() == aId)
		{
			id = static_cast<int>(i);
			return myTimers[id];
		}
	}
	ASSERT(false, "Couldn't find timer with specified id");
	return myTimers[-1];
}

const int TimerManager::CreateTimer()
{
	Timer t;
	t.SetID(myTimers.Size());
	myTimers.Add(t);
	
	return t.GetId();
}

const int TimerManager::CreateAndStartTimer()
{
	Timer t;
	t.Start();
	myTimers.Add(t);

	return t.GetId();
}