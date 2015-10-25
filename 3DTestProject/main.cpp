#include <Windows.h>
#include <ZMasherMain.h>


int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nCmdShow)
{
	ZMasherMain::Instance()->Init();

	while (ZMasherMain::Instance()->Update())
	{
		Sleep(1);
	}
	return 0;
}