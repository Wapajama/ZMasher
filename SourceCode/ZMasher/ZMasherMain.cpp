#include "ZMasherMain.h"
#include <Math/ZMVector.h>
#include <Math/Vector2decl.h>
#include <Windows.h>
#include <Time\TimerManager.h>
#include "GameplayState.h"
#include <ModelViewerState.h>
#include <CameraState.h>
#include <ZMasher\InputManager.h>

#include <iostream>
#include <fstream>

#include <DataStructures\BinarySearchTree.h>
#include <ZMUtils\Utility\ZMasherUtilities.h>
#include <ZMUtils\File\ZMXML.h>
#include <ZMUtils\File\PathManager.h>

#include <tinyxml2.h>
#include "modelviewer.h"
#include <Modelviewer/GeneratedFiles/ui_modelviewerwindow.h>
#include <iostream>

#include <ZMasherGfxDX11/Camera.h>
#include <qapplication.h>
#include <QGraphicsProxyWidget>
#include <thread>

#include <Time/Profiler.h>

class IntComparer
	:public ZMasher::BSTComparator<int>
{
public:
	bool LessThan(const int& one, const int& two)const override
	{
		return one < two;
	}
	bool GreaterThan(const int& one, const int& two)const override
	{
		return one > two;
	}
	bool Equals(const int& one, const int& two)const override
	{
		return one == two;
	}
};

//ZMasher::BinarySearchTree<int, IntComparer> test;
//void BinaryTreeTest()
//{
//	int derp = 0;
//	for (int i = 0; i < 100000; i++)
//	{
//		derp= ZMasher::GetRandomInt(-100000, 100000);
//		test.Insert(derp);
//	}
//	test.TestIfCorrect();
//	ZMasher::BSTNode<int, IntComparer>* test_node = test.Find(derp);
//	test_node =nullptr;
//	while(test_node == nullptr)
//	{
//		test_node = test.Find(ZMasher::GetRandomInt(-1000, 1000));
//	}
//	test.TestIfCorrect();
//}

using namespace ZMasher;

LRESULT CALLBACK ZMasherWinProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

QApplication* g_Application;
ZMasherMain::ZMasherMain()
	:m_Camera(nullptr),
	m_QApplicationThread(nullptr)
{
}

ZMasherMain::~ZMasherMain()
{
	if (m_QApplicationThread)
	{
		m_QApplicationThread->join();
	}
	delete m_QApplicationThread;
	
	#ifdef BENCHMARK
	Profiler::Instance()->AddTimeStamp(m_TotalFrame, "TotalFrame");
	Profiler::Instance()->AddTimeStamp(m_RenderFrame, "Render");
	Profiler::Instance()->AddTimeStamp(m_LogicFrame, "Logic");
	Profiler::Instance()->AddTimeStamp(m_RenderInternalFrame, "RenderInternal");
#endif // BENCHMARK
	m_Renderer.Destroy();
	m_StateStack.Exit();
	Profiler::Release();
}

ZMasherMain* ZMasherMain::Instance()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new ZMasherMain();
	}
	return m_Instance;
}

void QApplicationThread()
{
	g_Application->exec();
	g_Application->exit();
}

bool ZMasherMain::Init(ZMasherInitInfo info)
{
	m_InitInfo = info;
	PathManager::Create();
	ReadInitFile();
	Profiler::Create();

	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	m_Camera = new Camera(Vector2<int>(m_WinVals.m_Resolution.x,
		m_WinVals.m_Resolution.y));

	if (m_Camera != nullptr)
	{
		m_Camera->SetPosition(Vector3f(0, 0, 0));
	}

	InputManager::Create(GetModuleHandle(NULL), m_WinVals.m_WindowHandle, m_WinVals.m_Resolution.x, m_WinVals.m_Resolution.y);
	if (m_InitInfo.m_Args != nullptr &&
		strcmp(m_InitInfo.m_Args, "modelviewer") == 0)
	{
		//m_GameState = new ModelViewerState();
		int argc = 0;
		char* argv = 0;
		g_Application = new QApplication(argc, &argv);

		m_QApplicationThread = new std::thread(QApplicationThread);

		ModelViewerState* modelViewerState = new ModelViewerState(m_Camera);
		modelViewerState->Init(m_InitInfo.m_Args);

		m_WinVals.m_WindowHandle = (HWND)modelViewerState->GetUI()->graphicsView->winId();

		CameraState* cameraState = new CameraState();
		cameraState->SetCamera(m_Camera);
		cameraState->Init(m_InitInfo.m_Args);

		m_StateStack.PushState(modelViewerState);
		m_StateStack.PushState(cameraState);

		InitWindowClass();
		CreateWinApiWindow();

		const bool test = CreateD3D();
		ASSERT(test, "D3D Failed to init!");

		m_Renderer.Init(m_D3DInterface, Profiler::Instance(), TimerManager::GetInstance(),PathManager::Instance());
		m_Renderer.SetCamera(m_Camera);

	} else
	{
		InitWindowClass();
		CreateWinApiWindow();

		const bool test = CreateD3D();
		ASSERT(test, "D3D Failed to init!");

		m_Renderer.Init(m_D3DInterface, Profiler::Instance(), TimerManager::GetInstance(), PathManager::Instance());
		m_Renderer.SetCamera(m_Camera);

		GameplayState* gameState = new GameplayState(m_Camera);
		gameState->Init(nullptr);
		m_StateStack.PushState(gameState);
	}
	TimerManager::GetInstance()->Update();

	m_LoopTimer = TimerManager::GetInstance()->CreateAndStartTimer();

	m_MouseTrackEvent.cbSize = sizeof(m_MouseTrackEvent);
	m_MouseTrackEvent.dwFlags = TME_LEAVE | TME_HOVER | TME_NONCLIENT;
	//Retrieves the time, in milliseconds, that the mouse pointer has to stay in the hover rectangle for TrackMouseEvent to generate a WM_MOUSEHOVER message. 
	//The pvParam parameter must point to a UINT variable that receives the time.
	UINT hoverTime = 400;
	SystemParametersInfo(SPI_GETMOUSEHOVERTIME, hoverTime, NULL, 0);
	m_MouseTrackEvent.dwHoverTime = hoverTime;
	m_MouseTrackEvent.hwndTrack = m_WinVals.m_WindowHandle;
#ifdef BENCHMARK
	Profiler::Instance()->StartBenchmark();
#endif // BENCHMARK

	return true;
}

bool ZMasherMain::Update()
{
	if (HandleWinMsg() == false)
	{
		return false;
	}

	TimerManager::GetInstance()->GetMainTimer().Update();
	float dt = min(static_cast<float>(TimerManager::GetInstance()->GetMainTimer().TimeSinceLastFrame().GetSeconds()), 0.032f);

#ifdef BENCHMARK
	const bool benchmark = Profiler::Instance()->IterateFrame(dt);
	if (!benchmark)
	{
		return false;
	}
#endif // BENCHMARK_TEST

	///TrackMouseEvent(&m_MouseTrackEvent);
	InputManager::Instance()->Update(dt);

#ifdef BENCHMARK
	m_TotalFrame.StartTimeStamp();
	m_RenderFrame.StartTimeStamp();
#endif // BENCHMARK

	Render(dt);

#ifdef BENCHMARK
	m_RenderFrame.EndTimeStamp();
	m_LogicFrame.StartTimeStamp();
#endif // BENCHMARK

	//RECT window_rect;
	//GetWindowRect(m_WinVals.m_WindowHandle, &window_rect);
	//reinterpret_cast<GameplayState*>(m_GameState)->m_PrevMousePos = ZMasher::Vector2i((window_rect.left + window_rect.right) / 2, (window_rect.top + window_rect.bottom) / 2);
	if (!m_StateStack.Update(dt))
	{
#ifdef BENCHMARK
#endif // BENCHMARK
		//ASSERT(false, "!m_StateStack.Update(dt)");
		//return false;
	}
#ifdef BENCHMARK
	m_LogicFrame.EndTimeStamp();
	m_TotalFrame.EndTimeStamp();
#endif // BENCHMARK

	//SetCursorPos((window_rect.left + window_rect.right)/ 2, (window_rect.top + window_rect.bottom) / 2);
	return true;
}

void ZMasherMain::Destroy()
{
	delete m_Instance;
}

bool ZMasherMain::HandleWinMsg()
{
	if (PeekMessage(&m_WinVals.m_Message, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&m_WinVals.m_Message);
		DispatchMessage(&m_WinVals.m_Message);

		if (m_WinVals.m_Message.message == WM_QUIT)
			return false;
	}
	return true;
}

bool ZMasherMain::ReadInitFile()
{
	std::string init_file = PathManager::Instance()->GetDataPath() + "init.xml";

	ZMXML initReader(init_file);

	m_WinVals.m_Fullscreen = initReader.GetBoolValue("fullscreen");
	m_WinVals.m_ScreenDepth = initReader.GetFloatValue("ScreenDepth");
	m_WinVals.m_ScreenNear = initReader.GetFloatValue("ScreenNear");
	m_WinVals.m_VSync = initReader.GetBoolValue("vsync");

	//std::wstring w_TitlebarName = 
	m_WinVals.m_TitleBarName = reinterpret_cast<LPCSTR>(initReader.GetStringValue("titlebarname"));
	const Vector2i res = initReader.GetVec2IValue("resolution");
	m_WinVals.m_Resolution.x = res.x;
	m_WinVals.m_Resolution.y = res.y;
	return true;
}

void ZMasherMain::InitWindowClass()
{
	//this works on Windows 7 and windows 8
	m_WinVals.m_ExtWindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	m_WinVals.m_ExtWindowClass.lpfnWndProc = ZMasherWinProc;
	m_WinVals.m_ExtWindowClass.cbClsExtra = 0;
	m_WinVals.m_ExtWindowClass.cbWndExtra = 0;
	m_WinVals.m_ExtWindowClass.hInstance = GetModuleHandle(NULL);
	m_WinVals.m_ExtWindowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	m_WinVals.m_ExtWindowClass.hIconSm = m_WinVals.m_ExtWindowClass.hIcon;
	m_WinVals.m_ExtWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	m_WinVals.m_ExtWindowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	m_WinVals.m_ExtWindowClass.lpszMenuName = NULL;
	m_WinVals.m_ExtWindowClass.lpszClassName = m_WinVals.m_TitleBarName;
	m_WinVals.m_ExtWindowClass.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&m_WinVals.m_ExtWindowClass);
}

void ZMasherMain::CreateWinApiWindow()
{
	POINT windowDims = m_WinVals.m_Resolution;

	RECT windowRect = { 0, 0, windowDims.x,windowDims.y };

	DWORD windowStyleEx = WS_EX_CLIENTEDGE;
	DWORD windowStyle = WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;

	AdjustWindowRectEx(&windowRect, windowStyle, false, windowStyleEx);

	if (m_WinVals.m_WindowHandle == 0)
	{
		m_WinVals.m_WindowHandle = CreateWindowEx(windowStyleEx,
			m_WinVals.m_TitleBarName,
			m_WinVals.m_TitleBarName,
			windowStyle,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			windowDims.x,
			windowDims.y,
			NULL,
			NULL,
			m_WinVals.m_ExtWindowClass.hInstance,
			NULL);
	}


	//when does this return false? 
	//It sure as hell isn't when it failed to open the window :p
	ShowWindow(m_WinVals.m_WindowHandle, 1);

}

bool ZMasherMain::CreateD3D()
{
	const bool test = m_D3DInterface.Init(m_WinVals);
	if (test == false)
	{
		assert(test);
		return false;
	}
	return true;
}

void ZMasherMain::Render(const float dt)
{
	m_D3DInterface.BeginScene();
	m_Renderer.Render(m_D3DInterface, dt);

#ifdef BENCHMARK
	m_RenderInternalFrame.StartTimeStamp();
	m_D3DInterface.EndScene();
	m_RenderInternalFrame.EndTimeStamp();
#else
	m_D3DInterface.EndScene();
#endif // BENCHMARK

}

LRESULT CALLBACK ZMasherWinProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_KEYDOWN:

		// If ESC key is pressed, close the app
		if (wparam == VK_ESCAPE)
		{
			SendMessage(hwnd, WM_CLOSE, 0, 0);
		}

		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_MOUSEHOVER:
		//m_MouseTrackEvent.dwFlags = TME_LEAVE | TME_HOVER | TME_NONCLIENT;
		InputManager::Instance()->CursorInsideClientWindow(true);
		return 0;
	case WM_NCMOUSEHOVER:
		//m_MouseTrackEvent.dwFlags = TME_LEAVE | TME_HOVER;
		InputManager::Instance()->CursorInsideClientWindow(false);
		return 0;
	case WM_MOUSELEAVE:
		//m_MouseTrackEvent.dwFlags = TME_LEAVE | TME_HOVER | TME_NONCLIENT;
		InputManager::Instance()->CursorInsideClientWindow(false);
	case WM_NCMOUSELEAVE:
		//m_MouseTrackEvent.dwFlags = TME_LEAVE | TME_HOVER;
		InputManager::Instance()->CursorInsideClientWindow(true);
		return 0;
	case WM_KILLFOCUS:
		InputManager::Instance()->ClientInFocus(false);
		return 0;
	case WM_SETFOCUS:
		InputManager::Instance()->ClientInFocus(true);
		return 0;

	}

	return DefWindowProc(hwnd, message, wparam, lparam);
}

ZMasherMain* ZMasherMain::m_Instance = nullptr;