#pragma once
#include "DataStructures\GrowArray.h"
#include <Utility\ZMSingleton.h>
#include "../GlobalIncludes/project_defines.h"//TODO: Remove relative paths
#include <string>

struct ProfilerTaskID
{
	int m_TaskID;//index to array
};

class Profiler
	: public ZMSingleton<Profiler>
{
public:

	bool IterateFrame(const float dt);//false means benchmarktime has run out, time to finish benchmarking

	ProfilerTaskID AddTask(const char* name);

	void BeginTask(ProfilerTaskID id);
	void EndTask(ProfilerTaskID id);

protected:
	friend class ZMSingleton<Profiler>;
	Profiler();
	~Profiler();

private:
	float m_BenchmarkTime;
	int m_TimerIndex;
	struct ProfilerTask
	{
		std::string m_Name;
		double m_TotalTime;
	};

	struct TimeStamp
	{
		double m_ElapsedTime;
		ProfilerTaskID m_ID;
	};

	GrowArray<ProfilerTask> m_Tasks;
	GrowArray<TimeStamp> m_TimeStampStack;

	int m_NOGameObjects;
	float m_AvgGameObjects;

	void FinishBenchmark();//output results to file

	int m_NumberOfFrames;
};

