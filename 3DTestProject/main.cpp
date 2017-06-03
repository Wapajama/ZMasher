#include <Windows.h>
#include <ZMasherMain.h>
#include "DataStructures\GrowArray.h"
#include <time.h>

int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nCmdShow)
{
	srand(time(0));

	const bool test = ZMasherMain::Instance()->Init();

	assert(test);

	while (ZMasherMain::Instance()->Update())
	{
	}
	ZMasherMain::Destroy();
	
	return 0;
}