#include "ZMasherMain.h"
#include <Math/ZMVector.h>
#include <Math/Vector2decl.h>
#include <Windows.h>
#include <Time\TimerManager.h>
#include "GameplayState.h"
using namespace ZMasher;

LRESULT CALLBACK ZMasherWinProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

ZMasherMain::ZMasherMain()
{
	m_WinVals.m_TitleBarName = reinterpret_cast<LPCWSTR>(ZMASHER_TITLE_BAR_NAME);
	m_WinVals.m_Resolution.x = 1280;
	m_WinVals.m_Resolution.y = 720;
}

ZMasherMain::~ZMasherMain()
{
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
	InitWindowClass();
	CreateWinApiWindow();
	const bool test = CreateD3D();
	assert(test);

	m_Camera = new Camera(Vector2<int>(m_WinVals.m_Resolution.x,
									   m_WinVals.m_Resolution.y));
	m_Camera->SetPosition(Vector3f(0, 0, 10.f));

	m_Renderer.Init(m_D3DInterface);
	m_Renderer.SetCamera(m_Camera);
	m_GameState = new GameplayState();
	m_GameState->Init(nullptr);
	//bool lerpDErp = false;
	//GameObject test_derp;
	//for (short i = 0; i < 100; i++)
	//{
	//	test_derp = m_GameObjectManager.CreateGameObject(1);
	//	lerpDErp = m_GameObjectManager.IsAlive(test_derp);
	//}
	//TimerManager::GetInstance()->Update();
	//GM_TOGGLE_ALIVE_GO(test_derp.m_ID);
	//lerpDErp = m_GameObjectManager.IsAlive(test_derp);
	//GM_TOGGLE_ALIVE_GO(test_derp.m_ID);
	//lerpDErp = m_GameObjectManager.IsAlive(test_derp);
	return true;
}

bool ZMasherMain::Update()
{
	if (HandleWinMsg() == false)
	{
		return false;
	}
	const float dt = static_cast<double>(TimerManager::GetInstance()->GetMainTimer().TimeSinceLastFrame().GetSeconds());//TODO: optimize dis
	Render();
	if (!m_GameState->Update(dt))
	{
		return false;
	}
	return true;
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
	return true;
}

void ZMasherMain::Render()
{
	m_D3DInterface.BeginScene();

	m_Renderer.Render(m_D3DInterface);
	
	m_D3DInterface.EndScene();
}

const float global_speed = 900.f;

void ZMasherMain::MoveForward()
{
	m_Camera->SetPosition(m_Camera->GetPosition() + Vector3f(0,0, -global_speed * 0.016f));
}

void ZMasherMain::MoveBackwards()
{
	m_Camera->SetPosition(m_Camera->GetPosition() + Vector3f(0,0, global_speed * 0.016f));
}

void ZMasherMain::MoveRight()
{
	m_Camera->SetPosition(m_Camera->GetPosition() + Vector3f(-global_speed * 0.016f, 0,0));
}

void ZMasherMain::MoveLeft()
{
	m_Camera->SetPosition(m_Camera->GetPosition() + Vector3f(global_speed * 0.016f, 0,0));
}

const float global_rotation_speed = 0.1f;

void ZMasherMain::RotateRight()
{
	m_Camera->RotateY(global_rotation_speed);
}

void ZMasherMain::RotateLeft()
{
	m_Camera->RotateY(-global_rotation_speed);
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

		/*
			TODO: replace this with legitimate gameplay code
		*/
		if (wparam == 'W')
		{
			ZMasherMain::Instance()->MoveForward();
		}
		if(wparam == 'S')
		{
			ZMasherMain::Instance()->MoveBackwards();
		}
		if (wparam == 'A')
		{
			ZMasherMain::Instance()->MoveLeft();
		}
		if (wparam == 'D')
		{			
			ZMasherMain::Instance()->MoveRight();
		}
		if (wparam == VK_LEFT)
		{
			ZMasherMain::Instance()->RotateLeft();				
		}
		if (wparam == VK_RIGHT)
		{
			ZMasherMain::Instance()->RotateRight();
		}

		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wparam, lparam);
}

ZMasherMain* ZMasherMain::m_Instance = nullptr;