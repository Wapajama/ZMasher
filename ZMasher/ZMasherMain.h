#pragma once
#include "dllHeader.h"
#include "ZMD3DInterface.h"
#include "ZMAPIStructs.h"
#include "Camera.h"
#include "ZMModel.h"
#include "ZMRenderer.h"
#include <Profiler.h>

#define ZMASHER_TITLE_BAR_NAME L"ZMasher"

class GameState;

class ZMASHER_DLL ZMasherMain
{
public:

	static ZMasherMain* Instance();

	bool Init();
	bool Update();
	static void Destroy();

	/*
		TODO: remove and replace with legitimate game code
	*/

	inline ZMD3DInterface* GetD3DInterface();

private:

	void Render(const float dt);

	ZMasherMain();
	~ZMasherMain();

	static ZMasherMain* m_Instance;

	ZMWinApiCointainer m_WinVals;
	ZMD3DInterface m_D3DInterface;

	Camera* m_Camera;//TODO: redo design of this 
	ZMRenderer m_Renderer;
	GameState* m_GameState;
	
#ifdef BENCHMARK
	ProfilerTaskID m_TotalFrame;
	ProfilerTaskID m_LogicFrame;
	ProfilerTaskID m_RenderFrame;
	ProfilerTaskID m_RenderInternalFrame;
#endif // BENCHMMARK


	void InitWindowClass();
	void CreateWinApiWindow();
	bool CreateD3D();

	bool HandleWinMsg();

};

inline ZMD3DInterface* ZMasherMain::GetD3DInterface()
{
	return &m_D3DInterface;
}