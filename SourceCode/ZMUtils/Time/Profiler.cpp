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

#ifdef __CLANG
	std::string compiler = "CLANG";
#else
	std::string compiler = "DEFAULT - MSVC";
#endif // __CLANG


	//overall time
#ifdef _DEBUG
	fout << "----- Benchmark Results DEBUG (COMPILER: " << compiler << ") -------" << std::endl;
#else
	fout << "----- Benchmark Results (COMPILER: " << compiler << ") -------" << std::endl;
#endif // DEBUG

	fout << std::endl;

	TimerManager::GetInstance()->GetTimer(m_TimerIndex).Update();
	const double total_time = TimerManager::GetInstance()->GetTimer(m_TimerIndex).TimeSinceStart().GetSeconds();
	const double n_Frames = static_cast<double>(m_NumberOfFrames);
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

	fout << std::endl;
	for (int i = 0; i < m_TimeStamps.Size(); i++)
	{
		for (int j = i +1; j < m_TimeStamps.Size(); j++)
		{
			if (m_TimeStamps[i].GetTime() < m_TimeStamps[j].GetTime())
			{
				TaggedTimeStamp task = m_TimeStamps[i];
				m_TimeStamps[i] = m_TimeStamps[j];
				m_TimeStamps[j] = task;
			}
		}
	}

	fout << "--- Total time per task ---" << std::endl;
	for (short i = 1; i < m_TimeStamps.Size(); i++)
	{
		fout << "Task :" 
			<< m_TimeStamps[i].GetTime()
			<< " " << time_unit
			<< " (" << (m_TimeStamps[i].GetTime() / total_time) * 0.1f // 0.1 bc GetTime: ms, total_time: s, to %
			<< "\% of total)"
			<< "Name:\""
			<< m_TimeStamps[i].m_Name
			<< "\""
			<< std::endl;
	}

	fout << std::endl;
	fout << "--- Avarage time per frame ---" << std::endl;
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

	for (int i = 0; i < m_TimeStamps.Size(); i++)
	{
		for (int j = i +1; j < m_TimeStamps.Size(); j++)
		{
			if (m_TimeStamps[i].m_TimeStamp.GetAverageTimeStamp() < m_TimeStamps[j].m_TimeStamp.GetAverageTimeStamp())
			{
				TaggedTimeStamp task = m_TimeStamps[i];
				m_TimeStamps[i] = m_TimeStamps[j];
				m_TimeStamps[j] = task;
			}
		}
	}

	fout << "--- Avarage time per TimeStamp ---" << std::endl;
	for (short i = 1; i < m_TimeStamps.Size(); i++)
	{
		fout << "Task :" 
			<< m_TimeStamps[i].m_TimeStamp.GetAverageTimeStamp()
			<< " " << time_unit
			<< " (" << (m_TimeStamps[i].m_TimeStamp.GetAverageTimeStamp() / (total_time/n_Frames)) * 0.1f
			<< "\% of total)"
			<< "Name:\""
			<< m_TimeStamps[i].m_Name
			<< "\""
			<< std::endl;
	}

	fout << std::endl;

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

#endif // BENCHMARK

}
