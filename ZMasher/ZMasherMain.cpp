#include "ZMasherMain.h"


LRESULT CALLBACK ZMasherWinProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

ZMasherMain::ZMasherMain()
{
	Vector2f testCompile;
	m_WinVals.m_TitleBarName = reinterpret_cast<LPCWSTR>(ZMASHER_TITLE_BAR_NAME);
	testCompile += Vector2f(0,1);
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
		Render();
		Sleep(1);

	}
	return true;
}

bool ZMasherMain::Init()
{
	InitWindowClass();
	CreateViewPort();
	const bool test = CreateD3D();
	assert(test);

	m_Camera = new CameraClass();
	m_Camera->SetPosition(Vector3f(0, 0, -10.f));

	m_Model = new ModelClass();
	m_Model->Init(m_D3DInterface.GetDevice());

	m_Shader = new ColorClassShader();

	const bool test2 = m_Shader->Init(m_D3DInterface.GetDevice(),
				   m_WinVals.m_WindowHandle);

	assert(test2);

	return true;
}

void ZMasherMain::InitWindowClass()
{
	//m_WinVals.m_ExtWindowClass.cbSize = sizeof(WNDCLASSEX);
	//m_WinVals.m_ExtWindowClass.style = CS_HREDRAW | CS_VREDRAW;
	//m_WinVals.m_ExtWindowClass.lpfnWndProc = ZMasherWinProc;
	//m_WinVals.m_ExtWindowClass.hInstance = GetModuleHandle(NULL);
	//m_WinVals.m_ExtWindowClass.lpszClassName = m_WinVals.m_TitleBarName;
	//m_WinVals.m_ExtWindowClass.hCursor = (HCURSOR)(LoadImage(NULL, MAKEINTRESOURCE(IDC_ARROW), IMAGE_CURSOR, 0, 0, LR_SHARED));

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

void ZMasherMain::CreateViewPort()
{
	Vector2i windowDims(m_WinVals.m_Resolution.x,
						m_WinVals.m_Resolution.y);

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
	bool result = ShowWindow(m_WinVals.m_WindowHandle, 1);
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
	DirectX::XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;
	m_D3DInterface.BeginScene();
	
	m_Camera->Render();

	m_D3DInterface.GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3DInterface.GetProjectionMatrix(projectionMatrix);

	m_Model->Render(m_D3DInterface.GetContext());

	m_Shader->Render(m_D3DInterface.GetContext(), m_Model->GetIndexCount(),
					 worldMatrix,
					 viewMatrix,
					 projectionMatrix);

	m_D3DInterface.EndScene();
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