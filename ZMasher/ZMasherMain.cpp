#include "ZMasherMain.h"


LRESULT CALLBACK ZMasherWinProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

ZMasherMain::ZMasherMain()
{
	Vector2f testCompile;
	m_WinVals.m_TitleBarName = reinterpret_cast<LPCWSTR>(ZMASHER_TITLE_BAR_NAME);
	testCompile += Vector2f(0,1);
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

bool ZMasherMain::Update()
{
	if (HandleWinMsg() == false)
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
	else
	{
		m_D3DInterface.Render();
		m_D3DInterface.Clear();
		Sleep(1);
	}
	return true;
}

bool ZMasherMain::Init()
{
	InitWindowClass();
	CreateViewPort();
	CreateD3D();

	return true;
}

void ZMasherMain::InitWindowClass()
{
	m_WinVals.m_ExtWindowClass.cbSize = sizeof(WNDCLASSEX);
	m_WinVals.m_ExtWindowClass.style = CS_HREDRAW | CS_VREDRAW;
	m_WinVals.m_ExtWindowClass.lpfnWndProc = ZMasherWinProc;
	m_WinVals.m_ExtWindowClass.hInstance = GetModuleHandle(NULL);
	m_WinVals.m_ExtWindowClass.lpszClassName = m_WinVals.m_TitleBarName;
	m_WinVals.m_ExtWindowClass.hCursor = (HCURSOR)(LoadImage(NULL, MAKEINTRESOURCE(IDC_ARROW), IMAGE_CURSOR, 0, 0, LR_SHARED));

	RegisterClassEx(&m_WinVals.m_ExtWindowClass);
}

void ZMasherMain::CreateViewPort()
{
	Vector2i windowDims(600, 800);

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
	ShowWindow(m_WinVals.m_WindowHandle, 1);
}

void ZMasherMain::CreateD3D()
{
	m_D3DInterface.Init(m_WinVals);
}

LRESULT CALLBACK ZMasherWinProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_KEYDOWN:

		// If ESC key is pressed, close the app
		if (wparam == VK_ESCAPE)
			SendMessage(hwnd, WM_CLOSE, 0, 0);

		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wparam, lparam);
}

ZMasherMain* ZMasherMain::m_Instance = nullptr;