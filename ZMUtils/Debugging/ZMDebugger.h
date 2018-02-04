#pragma once
#include "../Utility/ZMSingleton.h"
#include <assert.h>
void CrashProgram(const char* msg);

class ZMDebugger:
	public ZMSingleton<ZMDebugger>
{
public:

	void Assert(const bool condition, const char* msg);
	void OutputDebugMsg(const char* msg);//to consolewindow and VS output

private:

	void Crash(const char* msg);

	ZMDebugger();
	~ZMDebugger();
};

#ifndef GTEST_ZMASHER
#define ASSERT(condition, msg) ZMDebugger::Instance()->Assert(condition, msg)
#else
#define ASSERT(condition, msg) if(!(condition)) assert(false && msg);
#endif // !UNIT_TEST_CPP

#ifndef GTEST_ZMASHER
#define DEBUG_MSG(msg) ZMDebugger::Instance()->OutputDebugMsg(msg) 
#else
#define DEBUG_MSG(msg)
#endif // !GTEST_ZMASHER
