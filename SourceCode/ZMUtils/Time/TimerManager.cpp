#include "TimerManager.h"

TimerManager* TimerManager::m_Instance = nullptr;



TimerManager* TimerManager::GetInstance()
{
	if(m_Instance == nullptr)
	{
		m_Instance = new TimerManager();
	}

	return m_Instance;
}

void TimerManager::Create(TimerManager* instance)
{
	if (instance)
	{
		m_Instance = instance;
		return;
	}
	m_Instance = new TimerManager();
}

TimerManager::TimerManager(void):m_Timers(1024)
{
	m_MainTimer.Start();
}

TimerManager::~TimerManager(void)
{
}

void TimerManager::Update()
{
	m_MainTimer.Update();
	for(unsigned int i = 0; i < m_Timers.Size(); ++i)
	{
		m_Timers[i].Update();
	}
}

Timer& TimerManager::GetTimer(const int aId)
{
	int id = -1;

	for(int i = 0; i < m_Timers.Size(); ++i)
	{
		if(m_Timers[i].GetId() == aId)
		{
			id = static_cast<int>(i);
			return m_Timers[id];
		}
	}
	ASSERT(false, "Couldn't find timer with specified id");
	return m_Timers[-1];
}

const int TimerManager::CreateTimer()
{
	Timer t;
	t.SetID(m_Timers.Size());
	m_Timers.Add(t);
	
	return t.GetId();
}

const int TimerManager::CreateAndStartTimer()
{
	Timer t;
	t.Start();
	m_Timers.Add(t);

	return t.GetId();
}