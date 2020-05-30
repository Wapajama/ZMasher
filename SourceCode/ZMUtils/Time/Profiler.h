#pragma once
#include "DataStructures\GrowArray.h"
#include <Utility\ZMSingleton.h>
#include "../GlobalIncludes/project_defines.h"//TODO: Remove relative paths
#include <string>
#include <Time/Timer.h>

class Profiler
	: public ZMSingleton<Profiler>
{
public:

	bool IterateFrame(const float dt);//false means benchmarktime has run out, time to finish benchmarking

	inline void AddTimeStamp(const Timer& timeStamp, const char* name) { m_TimeStamps.Add({ timeStamp, name }); }

	void StartBenchmark();

protected:
	friend class ZMSingleton<Profiler>;
	Profiler();
	~Profiler();

private:
	float m_BenchmarkTime;
	int m_TimerIndex;

	struct TaggedTimeStamp
	{
		Timer m_TimeStamp;
		const char* m_Name;
		// Milliseconds
		const double GetTime()
		{
			return m_TimeStamp.GetTotalTimeStampTime().GetMilliseconds();
		}
	};

	GrowArray<TaggedTimeStamp> m_TimeStamps;

	int m_NOGameObjects;
	float m_AvgGameObjects;

	void FinishBenchmark();//output results to file

	int m_NumberOfFrames;
};

