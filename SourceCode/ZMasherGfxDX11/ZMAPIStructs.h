#pragma once
#include <Windows.h>
#include <ZMasherGfxDX12\dllHeader.h>
#include <Math/ZMVector.h>

struct ZMWinApiCointainer
{
	HWND m_WindowHandle;
	WNDCLASSEX m_ExtWindowClass;
	MSG m_Message;
	LPCSTR m_TitleBarName;

	//not winApi, Should make better interface?
	POINT m_Resolution;
	float m_ScreenDepth;
	float m_ScreenNear;
	bool m_VSync;
	bool m_Fullscreen;
};

struct ZMViewPort
{

};