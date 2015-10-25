#pragma once
#include "dllHeader.h"
#include <Windows.h>
#include <Vector\Vector.h>
#include "ZMD3DInterface.h"
#include "ZMAPIStructs.h"


#define ZMASHER_TITLE_BAR_NAME "ZMasher"

class ZMASHER_DLL ZMasherMain
{
public:

	static ZMasherMain* Instance();

	bool Init();
	bool Update();
	
private:


	ZMasherMain();
	~ZMasherMain();

	static ZMasherMain* m_Instance;

	ZMWinApiCointainer m_WinVals;
	ZMD3DInterface m_D3DInterface;

	void InitWindowClass();
	void CreateViewPort();
	void CreateD3D();

	bool HandleWinMsg();

};