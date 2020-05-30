#pragma once
#include <ZMasher\dllHeader.h>
#include <ZMasherGfxDX11/ZMD3DInterface.h>
#include <ZMasherGfxDX11/ZMAPIStructs.h>
#include <ZMasherGfxDX11/ZMModel.h>
#include <ZMasherGfxDX11/ZMRenderer.h>
#include <Time/Timer.h>
#include <StateStack.h>
#define ZMASHER_TITLE_BAR_NAME L"ZMasher"

class GameState;

struct ZMasherInitInfo
{
	const char* m_Args;
};

namespace std
{
	class thread;
}


class ZMASHER_DLL ZMasherMain
{

public:

	static ZMasherMain* Instance();

	bool Init(ZMasherInitInfo);
	bool Update();
	static void Destroy();

	inline ZMD3DInterface* GetD3DInterface();

private:

	int m_LoopTimer;

	ZMasherInitInfo m_InitInfo;
	void Render(const float dt);

	ZMasherMain();
	~ZMasherMain();

	static ZMasherMain* m_Instance;

	ZMWinApiCointainer m_WinVals;
	ZMD3DInterface m_D3DInterface;

	class Camera* m_Camera;//TODO: redo design of this 
	ZMRenderer m_Renderer;
	std::thread* m_QApplicationThread;

	StateStack m_StateStack;
	TRACKMOUSEEVENT m_MouseTrackEvent;
#ifdef BENCHMARK
	Timer m_TotalFrame;
	Timer m_LogicFrame;
	Timer m_RenderFrame;
	Timer m_RenderInternalFrame;
	// as a reference for knowing how much time is spent on the timer alone
	Timer m_NullTimeStamp; 
#endif // BENCHMMARK

	bool ReadInitFile();
	void InitWindowClass();
	void CreateWinApiWindow();
	bool CreateD3D();

	bool HandleWinMsg();

};

inline ZMD3DInterface* ZMasherMain::GetD3DInterface()
{
	return &m_D3DInterface;
}