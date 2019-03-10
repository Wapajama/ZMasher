#include <Windows.h>
#include <ZMasher\ZMasherMain.h>
#include <ZMUtils/Debugging/ZMDebugger.h>
#include <time.h>

int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nCmdShow)
{
	srand(1);
	ZMasherInitInfo info = {nullptr};
	const bool init = ZMasherMain::Instance()->Init(info);

	ASSERT(init, "ZMasher Failed to init");


	while (ZMasherMain::Instance()->Update())
	{
	}
	ZMasherMain::Destroy();
	return 0;
}