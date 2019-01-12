#include <Windows.h>
#include <ZMasher\ZMasherMain.h>
#include "DataStructures\GrowArray.h"
#include <time.h>
#include <MemoryManager.h>
#include <Debugging\ZMDebugger.h>

struct TestStruct
{
	union
	{
		unsigned __int64 m_ID;
		struct
		{
			// these three shorts are only made to "push up" the generation short
			short m_IDShort0;
			short m_IDShort1;
			short m_IDShort3;
			short m_Gen;
		};
	};
};

int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nCmdShow)
{
	//srand(time(0));
	srand(1);

	TestStruct test;
	test.m_ID = 1;
	//ZMasher::MemoryManager::CreateMemoryManager();

	const bool init = ZMasherMain::Instance()->Init();

	assert(init);


	while (ZMasherMain::Instance()->Update())
	{
	}
	ZMasherMain::Destroy();
	return 0;
}