#pragma once
#include "dllHeader.h"
#include <Windows.h>
#include <Vector\Vector.h>
#include "ZMD3DInterface.h"
#include "ZMAPIStructs.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "ColorClassShader.h"

#define ZMASHER_TITLE_BAR_NAME L"ZMasher"

class ZMASHER_DLL ZMasherMain
{
public:

	static ZMasherMain* Instance();

	bool Init();
	bool Update();

	/*
		TODO: remove and replace with legitimate game code
	*/
	
	void MoveForward();
	void MoveBackwards();
	void MoveRight();
	void MoveLeft();

	void RotateRight();
	void RotateLeft();

private:

	void Render();

	ZMasherMain();
	~ZMasherMain();

	static ZMasherMain* m_Instance;

	ZMWinApiCointainer m_WinVals;
	ZMD3DInterface m_D3DInterface;

	CameraClass* m_Camera;
	ModelClass* m_Model;
	ColorClassShader* m_Shader;

	void InitWindowClass();
	void CreateWinApiWindow();
	bool CreateD3D();

	bool HandleWinMsg();

};