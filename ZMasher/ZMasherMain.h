#pragma once
#include "dllHeader.h"
#include <Windows.h>

#include "ZMD3DInterface.h"
#include "ZMAPIStructs.h"
#include "CameraClass.h"
#include "ZMModel.h"
#include "ColorClassShader.h"
#include "TextureShaderClass.h"
#include "ZMRenderer.h"

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

	inline ZMD3DInterface* GetD3DInterface();

private:

	void Render();

	ZMasherMain();
	~ZMasherMain();

	static ZMasherMain* m_Instance;

	ZMWinApiCointainer m_WinVals;
	ZMD3DInterface m_D3DInterface;

	CameraClass* m_Camera;
	//ZMModel* m_Model;
	ZMRenderer m_Renderer;
	//ColorClassShader* m_Shader;

	//TextureShaderClass* m_TextureShader;

	void InitWindowClass();
	void CreateWinApiWindow();
	bool CreateD3D();

	bool HandleWinMsg();

};

inline ZMD3DInterface* ZMasherMain::GetD3DInterface()
{
	return &m_D3DInterface;
}