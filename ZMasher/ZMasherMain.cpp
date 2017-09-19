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
#include <ZMasher/ZMasherUtilities.h>

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

#include <iostream>

ZMasherMain::ZMasherMain()
	:m_Camera(nullptr)
{
	m_WinVals.m_TitleBarName = reinterpret_cast<LPCWSTR>(ZMASHER_TITLE_BAR_NAME);
	m_WinVals.m_Resolution.x = 1280;
	m_WinVals.m_Resolution.y = 720;
}

ZMasherMain::~ZMasherMain()
{
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

bool ZMasherMain::Init()
{
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

	m_Renderer.Init(m_D3DInterface);
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

	const float dt = static_cast<double>(TimerManager::GetInstance()->GetMainTimer().TimeSinceLastFrame().GetSeconds());//TODO: optimize dis

#ifdef BENCHMARK
	//const bool benchmark = Profiler::Instance()->IterateFrame(dt);
	//if (!benchmark)
	//{
	//	return false;
	//}
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
		Profiler::Instance()->EndTask(m_LogicFrame);
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

	//when does this return false? 
	//It sure as hell isn't when it failed to open the window :p
	ShowWindow(m_WinVals.m_WindowHandle, 1);

}

bool ZMasherMain::CreateD3D()
{
	m_WinVals.m_Fullscreen = false;
	m_WinVals.m_ScreenDepth = 1000.f;
	m_WinVals.m_ScreenNear = 0.1f;
	m_WinVals.m_VSync = true;

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
	
	Profiler::Instance()->BeginTask(m_RenderInternalFrame);
	m_D3DInterface.EndScene();
	Profiler::Instance()->EndTask(m_RenderInternalFrame);
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