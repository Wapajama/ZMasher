#include "Profiler.h"
#include <Time\TimerManager.h>
#include <fstream>
#include <iostream>
#include <Debugging\ZMDebugger.h>
#include <chrono>
#include <ctime>


#ifdef BENCHMARK

Profiler::Profiler()
	:m_TimeStampStack(32)
	,m_Tasks(32)
{
	m_NumberOfFrames = 0;
	m_TimerIndex = TimerManager::GetInstance()->CreateAndStartTimer();
	m_BenchmarkTime = 3.f;
}


Profiler::~Profiler()
{
	//FinishBenchmark();
}

ProfilerTaskID Profiler::AddTask(const char* name)
{
	for (short i = 0; i < m_Tasks.Size(); i++)
	{
		if (strcmp(m_Tasks[i].m_Name.c_str(), name) == 0)
		{
			return{ -1 };
		}
	}
	m_Tasks.Add({ name, 0 });
	return{ static_cast<int>(m_Tasks.Size() - 1) };
}

bool Profiler::IterateFrame(const float dt)
{
	++m_NumberOfFrames;
	m_BenchmarkTime -= dt;
	return m_BenchmarkTime >= 0;
}

void Profiler::BeginTask(ProfilerTaskID id)
{
	TimerManager::GetInstance()->GetTimer(m_TimerIndex).Update();
	const float start_time = TimerManager::GetInstance()->GetTimer(m_TimerIndex).TimeSinceStart().GetSeconds();
	m_TimeStampStack.Add({ start_time, id });
}
void Profiler::EndTask(ProfilerTaskID)
{
	//short index = -1;
	//if (m_TimeStampStack.back().m_ID.m_TaskID != id.m_TaskID)
	//{
	//	ASSERT(false, "Ended wrong task!");
	//}
	TimerManager::GetInstance()->GetTimer(m_TimerIndex).Update();
	const float end_time = TimerManager::GetInstance()->GetTimer(m_TimerIndex).TimeSinceStart().GetSeconds() - m_TimeStampStack.GetLast().m_ElapsedTime;
	m_Tasks[m_TimeStampStack.GetLast().m_ID.m_TaskID].m_TotalTime += end_time;
	m_TimeStampStack.RemoveLast();
}
#include <string> 
void Profiler::FinishBenchmark()
{
	time_t now = time(0);
	tm *ltm = localtime(&now);
	std::string file_name = "BenchmarkResult" +
		std::to_string(1900 + ltm->tm_year) +
		std::to_string(1 + ltm->tm_mon) +
		std::to_string(ltm->tm_mday) +
		std::to_string(1 + ltm->tm_hour) +
		std::to_string(1 + ltm->tm_min) +
		std::to_string(1 + ltm->tm_sec) +
		".txt";
	std::ofstream fout;

	fout.open(file_name);//save history? 
	const char* time_unit = "s";
	//overall time
	fout << "----- Benchmark Results -------" << std::endl;

	fout << std::endl;


	fout << "--- Total time per task ---" << std::endl;
	const double total_time = m_Tasks[0].m_TotalTime;
	for (short i = 1; i < m_Tasks.Size(); i++)
	{
		fout << "Task \"" << m_Tasks[i].m_Name.c_str() << "\": "
			<< m_Tasks[i].m_TotalTime
			<< " " << time_unit
			<< " (" << (m_Tasks[i].m_TotalTime / total_time) * 100.f
			<< "\% of total)"
			<< std::endl;
	}
	fout << std::endl;
	fout << "--- Avarage time per frame ---" << std::endl;
	const double n_Frames = static_cast<double>(m_NumberOfFrames);
	for (short i = 1; i < m_Tasks.Size(); i++)
	{
		fout << "Task \"" << m_Tasks[i].m_Name.c_str() << "\": "
			<< m_Tasks[i].m_TotalTime / n_Frames
			<< " " << time_unit
			<< " (" << (m_Tasks[i].m_TotalTime / total_time) * 100.f
			<< "\% of total)"
			<< std::endl;
	}

	fout << std::endl;

	fout << "--- Summary --- " << std::endl;
	fout << "Total: "
		<< total_time
		<< " " << time_unit
		<< std::endl;

	fout << "Avarage FPS: "
		<< 1.f / (total_time / n_Frames) 
		<< " FPS"
		<< std::endl;

	fout << std::endl;

	fout.flush();

	fout.close();
}

#endif // BENCHMARK
