#include <Windows.h>
#include <ZMasherMain.h>
#include "DataStructures\GrowArray.h"

int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nCmdShow)
{
	GrowArray<int> test2;

	test2[100] = 0;


	const bool test = ZMasherMain::Instance()->Init();

	assert(test);
	


	while (ZMasherMain::Instance()->Update())
	{
		Sleep(1);
	}
	return 0;
}