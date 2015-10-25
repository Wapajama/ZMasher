#pragma once
#include <d3d11.h>
#include "ZMVertexTypes.h"
#include "ZMAPIStructs.h"

/*
	TO DO: refactor this class to one with better name
	and more thought-through purpose? 

	NOTE: this here class is virtually copied from a tutorial
*/

class ZMD3DInterface
{
	typedef const ZMWinApiCointainer& ZMInitArgs;
public:
	ZMD3DInterface();
	~ZMD3DInterface();

	bool Init(ZMInitArgs args);
	void BeginScene();
	void EndScene();
	bool Render();
	bool Clear();
	void Release();

private:

	ID3D11DeviceContext* m_Context;
	ID3D11Device* m_Device;
	IDXGISwapChain* m_SwapChain;
	ID3D11RenderTargetView* m_RenderTarget;

	bool CreateDevice(ZMInitArgs args);
	bool CreateViewPort(ZMInitArgs args);


};

