#include "Profiler.h"
#include <Time\TimerManager.h>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <Debugging\ZMDebugger.h>
#include <chrono>
#include <ctime>
#include <string> 
#include <ZMasher\GameObjectManager.h> // TODO: NOT THIS


Profiler::Profiler()
	:
	m_TimeStamps(1024)
	//m_TimeStampStack(32)
	//, m_Tasks(32)
{
#ifdef BENCHMARK
	m_NumberOfFrames = 0;
	m_BenchmarkTime = 5.f;
#endif // BENCHMARK
}

Profiler::~Profiler()
{
#ifdef BENCHMARK
	FinishBenchmark();
#endif // BENCHMARK
}

void Profiler::StartBenchmark()
{
	m_TimerIndex = TimerManager::GetInstance()->CreateAndStartTimer();
}

//ProfilerTaskID Profiler::AddTask(const char* name)
//{
//#ifdef BENCHMARK
//	for (short i = 0; i < m_Tasks.Size(); i++)
//	{
//		if (strcmp(m_Tasks[i].m_Name.c_str(), name) == 0)
//		{
//			return{ -1 };
//		}
//	}
//	m_Tasks.Add({ name, 0 });
//	return{ static_cast<int>(m_Tasks.Size() - 1) };
//#endif // BENCHMARK
//	return { -1 };
//}

bool Profiler::IterateFrame(const float dt)
{
#ifdef BENCHMARK
	++m_NumberOfFrames;
	m_BenchmarkTime -= dt;
	m_NOGameObjects = static_cast<float>(GameObjectManager::Instance()->m_GameObjects.Size());

	m_AvgGameObjects = (((m_NumberOfFrames - 1)*(static_cast<float>(m_AvgGameObjects))) + m_NOGameObjects) / m_NumberOfFrames;

	return m_BenchmarkTime >= 0;
#else
	return false;
#endif // BENCHMARK
}

//void Profiler::BeginTask(ProfilerTaskID id)
//{
//#ifdef BENCHMARK
//	TimerManager::GetInstance()->GetTimer(m_TimerIndex).Update();
//	const float start_time = TimerManager::GetInstance()->GetTimer(m_TimerIndex).TimeSinceStart().GetSeconds();
//	m_TimeStampStack.Add({ start_time, id });
//#endif // BENCHMARK
//}
//
//void Profiler::EndTask(ProfilerTaskID)
//{
//#ifdef BENCHMARK
//	if (m_TimeStampStack.Size() > 0)
//	{
//		TimerManager::GetInstance()->GetTimer(m_TimerIndex).Update();
//		const float end_time = TimerManager::GetInstance()->GetTimer(m_TimerIndex).TimeSinceStart().GetSeconds() - m_TimeStampStack.GetLast().m_ElapsedTime;
//		m_Tasks[m_TimeStampStack.GetLast().m_ID.m_TaskID].m_TotalTime += end_time;
//		m_TimeStampStack.RemoveLast();
//	}
//#endif // BENCHMARK
//
//}

void Profiler::FinishBenchmark()
{
#ifdef BENCHMARK
	time_t now = time(0);
	tm *ltm = localtime(&now);
#ifdef _DEBUG
	std::string file_name = "BenchmarkResultDEBUG" +
#else
	std::string file_name = "BenchmarkResult" +
#endif // _DEBUG

		std::to_string(1900 + ltm->tm_year) +
		std::to_string(1 + ltm->tm_mon) +
		std::to_string(ltm->tm_mday) +
		std::to_string(1 + ltm->tm_hour) +
		std::to_string(1 + ltm->tm_min) +
		std::to_string(1 + ltm->tm_sec) +
		".txt";
	std::ofstream fout;

	fout.open(file_name);
	const char* time_unit = "ms";

	//overall time
#ifdef _DEBUG
	fout << "----- Benchmark Results DEBUG -------" << std::endl;
#else
	fout << "----- Benchmark Results -------" << std::endl;
#endif // DEBUG

	fout << std::endl;

	fout << "--- Total time per task ---" << std::endl;
	TimerManager::GetInstance()->GetTimer(m_TimerIndex).Update();
	const double total_time = TimerManager::GetInstance()->GetTimer(m_TimerIndex).TimeSinceStart().GetSeconds();

	//for (int i = 0; i < m_Tasks.Size(); i++)
	//{
	//	m_Tasks[i].m_TotalTime *= 1000.0; // Converting to miliseconds.
	//}
	//for (int i = 0; i < m_Tasks.Size(); i++)
	//{
	//	for (int j = i +1; j + 1 < m_Tasks.Size(); j++)
	//	{
	//		if (m_Tasks[i].m_TotalTime < m_Tasks[j].m_TotalTime)
	//		{
	//			ProfilerTask task = m_Tasks[i];
	//			m_Tasks[i] = m_Tasks[j];
	//			m_Tasks[j] = task;
	//		}
	//	}
	//}
	//for (short i = 1; i < m_Tasks.Size(); i++)
	//{
	//	if (m_Tasks[i].m_TotalTime == 0.f)
	//	{
	//		continue;
	//	}
	//	fout << "Task :" 
	//		<< m_Tasks[i].m_TotalTime
	//		<< " " << time_unit
	//		<< " (" << (m_Tasks[i].m_TotalTime / total_time) * 0.1f
	//		<< "\% of total)"
	//		<< "Name:\""
	//		<< m_Tasks[i].m_Name.c_str() 
	//		<< "\""
	//		<< std::endl;
	//}
	//fout << std::endl;
	//fout << "--- Avarage time per frame ---" << std::endl;
	//const double n_Frames = static_cast<double>(m_NumberOfFrames);
	//for (short i = 1; i < m_Tasks.Size(); i++)
	//{
	//	fout << "Task :" 
	//		<< m_Tasks[i].m_TotalTime / n_Frames
	//		<< " " << time_unit
	//		<< " (" << (m_Tasks[i].m_TotalTime / total_time) * 0.1f
	//		<< "\% of total)"
	//		<< "Name:\""
	//		<< m_Tasks[i].m_Name.c_str() 
	//		<< "\""
	//		<< std::endl;
	//}

	for (int i = 0; i < m_TimeStamps.Size(); i++)
	{
		for (int j = i +1; j + 1 < m_TimeStamps.Size(); j++)
		{
			if (m_TimeStamps[i].GetTime() < m_TimeStamps[j].GetTime())
			{
				TaggedTimeStamp task = m_TimeStamps[i];
				m_TimeStamps[i] = m_TimeStamps[j];
				m_TimeStamps[j] = task;
			}
		}
	}

	for (short i = 1; i < m_TimeStamps.Size(); i++)
	{
		//if (m_TimeStamps[i].GetTime() == 0.0)
		//{
		//	continue;
		//}
		fout << "Task :" 
			<< m_TimeStamps[i].GetTime()
			<< " " << time_unit
			<< " (" << (m_TimeStamps[i].GetTime() / total_time) * 0.1f
			<< "\% of total)"
			<< "Name:\""
			<< m_TimeStamps[i].m_Name
			<< "\""
			<< std::endl;
	}

	fout << std::endl;
	fout << "--- Avarage time per frame ---" << std::endl;
	const double n_Frames = static_cast<double>(m_NumberOfFrames);
	for (short i = 1; i < m_TimeStamps.Size(); i++)
	{
		fout << "Task :" 
			<< m_TimeStamps[i].GetTime() / n_Frames
			<< " " << time_unit
			<< " (" << (m_TimeStamps[i].GetTime() / total_time) * 0.1f
			<< "\% of total)"
			<< "Name:\""
			<< m_TimeStamps[i].m_Name
			<< "\""
			<< std::endl;
	}

	fout << std::endl;

	fout << "--- Summary --- " << std::endl;
	fout << "Total: "
		<< total_time
		<< " " << "s"
		<< std::endl;

	fout << "Avarage FPS: "
		<< 1.f / (total_time / n_Frames)
		<< " FPS"
		<< std::endl;

	fout << "Avarage n gameobjects: "
		<< m_AvgGameObjects
		<< std::endl;

	fout.flush();
	fout.close();

	STARTUPINFOA si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	std::string inputString = "C:\\Users\\Kristoffer\\Desktop ";

	inputString += file_name;

	CreateProcessA(
		"C:/Program Files (x86)/Notepad++/notepad++.exe",
		const_cast<char*>(inputString.c_str()),
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
		&pi             
	);

	//int retCode = system(args.c_str());
#endif // BENCHMARK

}
