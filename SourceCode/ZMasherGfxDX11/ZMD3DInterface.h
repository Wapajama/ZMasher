#pragma once
#include <C:\Users\Kristoffer\Documents\Visual Studio 2017\Projects\ZMasher\SourceCode\GlobalIncludes\project_defines.h>

#ifdef ZMASHER_DX12
#include <D3dx12.h>
#include <dxgi1_4.h>
#elif defined ZMASHER_DX11
#include <d3d11.h>
#else 
	#error No render API specified
#endif // ZMASHER_DX12

#include "ZMVertexTypes.h"
#include "ZMAPIStructs.h"
#include <DirectXMath.h>
#include <ZMasherGfxDX11\dllHeader.h>
//#include <Windows.h>
/*
	TO DO: refactor this class to one with better name
	and more thought-through purpose? 

	NOTE: this here class is virtually copied from a tutorial
*/

class ZMASHER_GFX_DX11_DLL ZMD3DInterface
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

#ifdef ZMASHER_DX12
	ID3D12Device* GetDevice();
	inline ID3D12CommandList* GetCommandList() { return m_CommandList; };
#else
	ID3D11DeviceContext* GetContext();
	ID3D11Device* GetDevice();
#endif // ZMASHER_DX12


private:

#ifdef ZMASHER_DX12
	ID3D12Device* m_Device;
	IDXGISwapChain* m_SwapChain;
	ID3D12DescriptorHeap* m_RenderTargetViewHeap;
	ID3D12DescriptorHeap* m_DepthStencilViewHeap;
	ID3D12Resource* m_BackBufferRenderTarget[2];

	ID3D12Resource* m_DepthStencilBuffer;

	ID3D12CommandQueue* m_CommandQueue;
	ID3D12CommandAllocator* m_CommandAllocator;
	ID3D12GraphicsCommandList* m_CommandList;

	ID3D12PipelineState* m_PipelineState;
	ID3D12Fence* m_Fence;
	HANDLE m_FenceEvent;
	unsigned int m_BufferIndex; 
	unsigned long long m_FenceValue;
	DXGI_FORMAT m_BackBufferFormat;
	bool m_4XMSAAState;
	UINT m_4XMSAAQuality;
	const UINT m_SwapChainBufferCount;
	UINT m_RtvDescriptorSize;
	UINT m_DsvDescriptorSize;
	UINT m_CbvSrvDescriptorSize;

	IDXGIFactory2* m_DxgiFactory2;

	D3D12_DESCRIPTOR_HEAP_DESC m_RtvHeapDesc;
	D3D12_DESCRIPTOR_HEAP_DESC m_DsvHeapDesc;

	int m_CurrentBackBuffer;
	tagRECT m_ScissorRect;

	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetBackBufferView(UINT index)const;

	D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilCPUHandle()const;

	enum BackBufferViewIndex:UINT
	{
		FRONT_
	};

	bool CreateCommandQueue(ZMInitArgs args);
	bool CreateDxgiFactory(ZMInitArgs args); //TODO: add this function for Dx11 as well?
	bool CreateSwapChain(ZMInitArgs args);   //^
	bool CreateDevice(ZMInitArgs args);
	bool CreateFence(ZMInitArgs args);
	bool CreateDescriptorHeap(ZMInitArgs args);
	bool CreateRenderTargetView(ZMInitArgs args);
	bool CreateDepthStencilTargetAndView(ZMInitArgs args);
#elif defined ZMASHER_DX11
	bool CreateDeviceAndSwapChain(ZMInitArgs args);
	ID3D11DeviceContext* m_Context;
	ID3D11Device* m_Device;
	IDXGISwapChain* m_SwapChain;
	ID3D11RenderTargetView* m_RenderTarget;
	ID3D11Texture2D* m_DepthStencilBuffer;
	ID3D11DepthStencilState* m_DepthStencilState;
	ID3D11DepthStencilView* m_DepthStencil;
	ID3D11RasterizerState* m_RasterizerState;
#endif // USE_DX11

	unsigned int m_Numerator  ;
	unsigned int m_Denominator;
	bool m_VSync;
	int m_VideoCardMemory;
	char m_VideoCardDesc[128];

	bool GetDisplayModeInfo(ZMInitArgs args);
	bool InitSwapChain(ZMInitArgs args, DXGI_SWAP_CHAIN_DESC& swap_chain_desc);
	bool CreateViewPort(ZMInitArgs args);
	bool CreateDepthStencil(ZMInitArgs args);
	bool CreateRasterizerState(ZMInitArgs args);
};

