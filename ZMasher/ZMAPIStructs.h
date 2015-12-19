#pragma once
#include <Windows.h>
#include "dllHeader.h"
#include <ZMVector.h>

struct ZMASHER_DLL ZMWinApiCointainer
{
	HWND m_WindowHandle;
	WNDCLASSEX m_ExtWindowClass;
	MSG m_Message;
	LPCWSTR m_TitleBarName;

	//not winApi, Should make better interface?
	POINT m_Resolution;
	float m_ScreenDepth;
	float m_ScreenNear;
	bool m_VSync;
	bool m_Fullscreen;
};

struct ZMASHER_DLL ZMViewPort
{

};