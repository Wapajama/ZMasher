#include "ZMasherMain.h"
#include <Math/ZMVector.h>
#include <Math/Vector2decl.h>
#include <Windows.h>
#include <Time\TimerManager.h>
#include "GameplayState.h"
#include <ZMasher\InputManager.h>

#include <iostream>
#include <fstream>

#include <DataStructures\BinarySearchTree.h>
#include <ZMUtils\Utility\ZMasherUtilities.h>
#include <ZMUtils\File\ZMXML.h>
#include <ZMUtils\File\PathManager.h>

#include <tinyxml2.h>
#include "modelviewer.h"
#include <qapplication.h>
#include <iostream>
#include <thread>
#include "C:\Users\Kristoffer\Documents\Visual Studio 2017\Projects\ZMasher\Executables\x64\Debug\ui_modelviewerwindow.h" // TODO: GO TO HELL FOR DOING THIS

class IntComparer
	:public ZMasher::BSTComparator<int>
{
public:
	bool LessThan(const int& one,const int& two)const override
	{
		return one < two;
	}
	bool GreaterThan(const int& one,const int& two)const override
	{
		return one > two;
	}
	bool Equals(const int& one,const int& two)const override
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


ZMModelViewer::ZMModelViewer()
{

}
ZMModelViewer::~ZMModelViewer()
{

}


void OnTestButton(bool checked)
{
	ASSERT(!checked, "Hello world!222");
}

void QApplicationThread()
{
	g_Application->exec();
}

ZMModelViewer* g_ZmModelViewer = nullptr;

ZMasherMain::ZMasherMain()
	:m_Camera(nullptr),
	m_ModelViewer(nullptr)
{
	int argc = 0;
	char* argv = 0;
	g_Application = new QApplication(argc, &argv);
	m_ModelViewer = new ModelViewer();
	g_ZmModelViewer = new ZMModelViewer();
	m_ModelViewer->show();
	m_ModelViewer->SetPushButtonClicked(&OnTestButton);
	m_QApplicationThread = new std::thread(QApplicationThread);
}

ZMasherMain::~ZMasherMain()
{
	Profiler::Release();
	g_Application->exit();
	m_QApplicationThread->join();
}

ZMasherMain* ZMasherMain::Instance()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new ZMasherMain();
	}
	return m_Instance;
}

bool ZMasherMain::Init()
{	
	PathManager::Create();
	ReadInitFile();
	Profiler::Create();

	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	//BinaryTreeTest();

	

#ifdef BENCHMARK
	m_TotalFrame = Profiler::Instance()->AddTask("TotalFrame");
	m_RenderFrame = Profiler::Instance()->AddTask("Render");
	m_LogicFrame = Profiler::Instance()->AddTask("Logic");
	m_RenderInternalFrame = Profiler::Instance()->AddTask("RenderInternal");
#endif // BENCHMARK
	
	InitWindowClass();
	CreateWinApiWindow();
	const bool test = CreateD3D();
	assert(test);

	m_Camera = new Camera(Vector2<int>(m_WinVals.m_Resolution.x,
									   m_WinVals.m_Resolution.y));

	
	if (m_Camera != nullptr)
	{
		m_Camera->SetPosition(Vector3f(0, 0, 10.f));
	}

	m_Renderer.Init(m_D3DInterface, Profiler::Instance(), TimerManager::GetInstance());
	m_Renderer.SetCamera(m_Camera);
	m_GameState = new GameplayState(m_Camera);
	m_GameState->Init(nullptr);

	TimerManager::GetInstance()->Update();
	return true;
}

bool ZMasherMain::Update()
{
	if (HandleWinMsg() == false)
	{
		return false;
	}
	TimerManager::GetInstance()->Update();

	const float dt = min(static_cast<float>(TimerManager::GetInstance()->GetMainTimer().TimeSinceLastFrame().GetSeconds()), 0.064);//TODO: optimize dis

#ifdef BENCHMARK
	const bool benchmark = Profiler::Instance()->IterateFrame(dt);
	if (!benchmark)
	{
		return false;
	}
#endif // BENCHMARK_TEST
	InputManager::Instance()->Update(dt);

#ifdef BENCHMARK
	Profiler::Instance()->BeginTask(m_TotalFrame);
	Profiler::Instance()->BeginTask(m_RenderFrame);
#endif // BENCHMARK

	Render(dt);

#ifdef BENCHMARK
	Profiler::Instance()->EndTask(m_RenderFrame);
	Profiler::Instance()->BeginTask(m_LogicFrame);
#endif // BENCHMARK

	//RECT window_rect;
	//GetWindowRect(m_WinVals.m_WindowHandle, &window_rect);
	//reinterpret_cast<GameplayState*>(m_GameState)->m_PrevMousePos = ZMasher::Vector2i((window_rect.left + window_rect.right) / 2, (window_rect.top + window_rect.bottom) / 2);
	if (!m_GameState->Update(dt))
	{
#ifdef BENCHMARK
		Profiler::Instance()->EndTask(m_LogicFrame);
#endif // BENCHMARK

		return false;
	}
#ifdef BENCHMARK
	Profiler::Instance()->EndTask(m_LogicFrame);
	Profiler::Instance()->EndTask(m_TotalFrame);
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

#define ZMASHER_MODELVIEWER

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


#ifdef ZMASHER_MODELVIEWER
	m_WinVals.m_WindowHandle = (HWND)m_ModelViewer->ui->graphicsView->winId();
#else
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
#endif // ZMASHER_MODELVIEWER


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
	
	InputManager::Create(GetModuleHandle(NULL), m_WinVals.m_WindowHandle, m_WinVals.m_Resolution.x, m_WinVals.m_Resolution.y);

	return true;
}

void ZMasherMain::Render(const float dt)
{
	m_D3DInterface.BeginScene();
	m_Renderer.Render(m_D3DInterface, dt);
	
#ifdef BENCHMARK
	Profiler::Instance()->BeginTask(m_RenderInternalFrame);
	m_D3DInterface.EndScene();
	Profiler::Instance()->EndTask(m_RenderInternalFrame);
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
	}

	return DefWindowProc(hwnd, message, wparam, lparam);
}

ZMasherMain* ZMasherMain::m_Instance = nullptr;