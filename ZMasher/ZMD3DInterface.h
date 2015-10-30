#pragma once
#include <d3d11.h>
#include "ZMVertexTypes.h"
#include "ZMAPIStructs.h"
#include <DirectXMath.h>
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
	ID3D11Texture2D* m_DepthStencilBuffer;
	ID3D11DepthStencilState* m_DepthStencilState;
	ID3D11DepthStencilView* m_DepthStencil;
	ID3D11RasterizerState* m_RasterizerState;
	
	bool m_VSync;
	int m_VideoCardMemory;
	char m_VideoCardDesc[128];

	unsigned int m_Numerator  ;
	unsigned int m_Denominator;

	DirectX::XMMATRIX m_ProjectionMatrix;
	DirectX::XMMATRIX m_WorldMatrix;
	DirectX::XMMATRIX m_OrthoganalMatrix;

	bool GetDisplayModeInfo(ZMInitArgs args);
	bool CreateDeviceAndSwapChain(ZMInitArgs args);
	bool InitDevice(ZMInitArgs args);
	bool InitSwapChain(ZMInitArgs args, DXGI_SWAP_CHAIN_DESC& swap_chain_desc);
	bool CreateViewPort(ZMInitArgs args);
	bool CreateDepthStencil(ZMInitArgs args);
	bool CreateRasterizerState(ZMInitArgs args);

};

