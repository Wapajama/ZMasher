#pragma once
#include <ZMasher\dllHeader.h>
#include <ZMasherGfxDX11/ZMD3DInterface.h>
#include <ZMasherGfxDX11/ZMAPIStructs.h>
#include "Camera.h"
#include <ZMasherGfxDX11/ZMModel.h>
#include <ZMasherGfxDX11/ZMRenderer.h>
#include <Time/Profiler.h>
#include <QtCore/qobject.h>
#define ZMASHER_TITLE_BAR_NAME L"ZMasher"

class GameState;

namespace std
{
	class thread;
}

struct ZMasherInitInfo
{
	const char* m_Args;
};

class ZMModelViewer : public QObject
{

public:
	ZMModelViewer();
	~ZMModelViewer();

};

class ZMASHER_DLL ZMasherMain
{

public:

	static ZMasherMain* Instance();

	bool Init();
	bool Update();
	static void Destroy();

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

	class ModelViewer* m_ModelViewer;
	
#ifdef BENCHMARK
	ProfilerTaskID m_TotalFrame;
	ProfilerTaskID m_LogicFrame;
	ProfilerTaskID m_RenderFrame;
	ProfilerTaskID m_RenderInternalFrame;
#endif // BENCHMMARK
	class ModelViewerWindow* m_ModelViewerWindow;
	std::thread* m_QApplicationThread;

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