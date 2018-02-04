#include "ZMDebugger.h"
#include <assert.h>
#include <Windows.h>
#include <string>

ZMDebugger::ZMDebugger()
{
#ifdef _DEBUG
	AllocConsole();
#endif
}

ZMDebugger::~ZMDebugger()
{
#ifdef _DEBUG
	FreeConsole();
#endif
}

void ZMDebugger::Crash(const char* msg)
{
	CrashProgram(msg);
}

void ZMDebugger::Assert(const bool condition, const char* msg)
{
	if (condition == false)
	{
		Crash(msg);
	}
}

void ZMDebugger::OutputDebugMsg(const char* msg)
{
	//darn, 2 heap allocations, remove asap(each string)
	std::string msg_short = msg;
	std::wstring w_msg(msg_short.begin(), msg_short.end());
	OutputDebugString(w_msg.c_str());	//VS output
	printf(msg);						//console window
}

void CrashProgram(const char* msg)
{
	MessageBoxA(NULL, msg, "Crash", MB_OK);
	abort();
}
