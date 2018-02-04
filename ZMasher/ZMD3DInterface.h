#pragma once
#include <project_defines.h>

#ifdef ZMASHER_DX12
#include <d3d12.h> 
#include <dxgi1_4.h>
#elif defined ZMASHER_DX11
#include <d3d11.h>
#else 
	#error No render API specified
#endif // ZMASHER_DX12

#include "ZMVertexTypes.h"
#include "ZMAPIStructs.h"
#include <DirectXMath.h>
#include <Windows.h>
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

	ID3D11DeviceContext* GetContext();
	ID3D11Device* GetDevice();

private:

#ifdef ZMASHER_DX12
	ID3D12Device* m_Device;
	ID3D12CommandQueue* m_CommandQueue;
	IDXGISwapChain3* m_SwapChain;
	ID3D12DescriptorHeap* m_RenderTargetViewHeap;
	ID3D12Resource* m_BackBufferRenderTarget[2];
	ID3D12CommandAllocator* m_CommandAllocator;
	ID3D12GraphicsCommandList* m_CommandList;
	ID3D12PipelineState* m_PipelineState;
	ID3D12Fence* m_Fence;
	HANDLE m_FenceEvent;
	unsigned int m_bufferIndex; 
	unsigned long long m_fenceValue;
#elif defined ZMASHER_DX11
	ID3D11DeviceContext* m_Context;
	ID3D11Device* m_Device;
	IDXGISwapChain* m_SwapChain;
	ID3D11RenderTargetView* m_RenderTarget;
	ID3D11Texture2D* m_DepthStencilBuffer;
	ID3D11DepthStencilState* m_DepthStencilState;
	ID3D11DepthStencilView* m_DepthStencil;
	ID3D11RasterizerState* m_RasterizerState;
	unsigned int m_Numerator  ;
	unsigned int m_Denominator;
#endif // USE_DX11

	bool m_VSync;
	int m_VideoCardMemory;
	char m_VideoCardDesc[128];

	bool GetDisplayModeInfo(ZMInitArgs args);
	bool CreateDeviceAndSwapChain(ZMInitArgs args);
	bool InitSwapChain(ZMInitArgs args, DXGI_SWAP_CHAIN_DESC& swap_chain_desc);
	bool CreateViewPort(ZMInitArgs args);
	bool CreateDepthStencil(ZMInitArgs args);
	bool CreateRasterizerState(ZMInitArgs args);
};

