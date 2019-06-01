#include "ZMD3DInterface.h"
#include <Debugging\ZMDebugger.h>

#define _USE_MATH_DEFINES
#include <math.h>

#define ZERO_MEM(var) ZeroMemory(&var, sizeof(decltype(var)))
#define RELEASE(ptr) ptr->Release();ptr = nullptr;
#define RETURN_IF_FAILED(hres) 	\
	if (FAILED(result))\
	{\
		assert(false);\
		return false;\
	}\

#define INIT(functionReturn) if(functionReturn == false)\
							{\
								assert(false);\
								return false;\
							}\

ZMD3DInterface::ZMD3DInterface()
#ifdef ZMASHER_DX12
	: m_SwapChainBufferCount(2)
#endif // ZMASHER_DX12
{
#ifdef ZMASHER_DX12
	m_Device = nullptr;
	m_CommandQueue = nullptr;
	m_SwapChain = nullptr;
	m_RenderTargetViewHeap = nullptr;
	m_BackBufferRenderTarget[2] = nullptr;
	m_CommandAllocator = nullptr;
	m_CommandList = nullptr;
	m_PipelineState = nullptr;
	m_Fence = nullptr;
	m_FenceEvent = nullptr;
	m_BufferIndex = 0;
	m_FenceValue = 0;
#elif defined ZMASHER_DX11
	m_SwapChain = nullptr;
	m_Device = nullptr;
	m_Context = nullptr;
	m_RenderTarget = nullptr;
	m_DepthStencilBuffer = nullptr;
	m_DepthStencilState = nullptr;
	m_DepthStencil = nullptr;
	m_RasterizerState = nullptr;
	m_Numerator = 0;
	m_Denominator = 0;
#endif // ZMASHER_DX1X
	m_VSync = false;
	m_VideoCardMemory = 0;
}

ZMD3DInterface::~ZMD3DInterface()
{
}

ID3D12Device* ZMD3DInterface::GetDevice()
{
	return m_Device;
}

void ZMD3DInterface::BeginScene()
{
	Clear();
}

void ZMD3DInterface::EndScene()
{
	Render();
}

bool ZMD3DInterface::Render()
{
	const int vsync = m_VSync ? 1 : 0;
	m_SwapChain->Present(0, 0);
	return true;
}

bool ZMD3DInterface::Init(ZMInitArgs args)
{
	m_VSync = args.m_VSync;

#ifdef ZMASHER_DX12
	INIT(CreateCommandQueue(args));
	INIT(CreateDxgiFactory(args)); 
	INIT(CreateSwapChain(args));
	INIT(CreateDevice(args));
	INIT(CreateFence(args));
	INIT(CreateDescriptorHeap(args));
	INIT(CreateRenderTargetView(args));
	INIT(CreateDepthStencilTargetAndView(args));
#else
	INIT(GetDisplayModeInfo(args));
	INIT(CreateDeviceAndSwapChain(args));
	INIT(CreateRasterizerState(args));
	INIT(CreateViewPort(args));
	INIT(CreateDepthStencil(args));
#endif // ZMASHER_DX12



	return true;
}

bool ZMD3DInterface::CreateCommandQueue(ZMInitArgs args)
{
	D3D12_COMMAND_QUEUE_DESC queue_desc = {};
	queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	HRESULT result = m_Device->CreateCommandQueue(&queue_desc, IID_PPV_ARGS(&m_CommandQueue));

	RETURN_IF_FAILED(result);

	result = m_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocator));

	RETURN_IF_FAILED(result);

	result = m_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CommandAllocator, nullptr, IID_PPV_ARGS(&m_CommandList));

	RETURN_IF_FAILED(result);

	m_CommandList->Close();

	return true;
}

bool ZMD3DInterface::CreateDxgiFactory(ZMInitArgs args)
{
	HRESULT result = CreateDXGIFactory1(IID_PPV_ARGS(&m_DxgiFactory2));
	RETURN_IF_FAILED(result);
	return true;
}

bool ZMD3DInterface::CreateSwapChain(ZMInitArgs args)
{
	m_BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(DXGI_SWAP_CHAIN_DESC));

	sd.BufferDesc.Width = args.m_Resolution.x;
	sd.BufferDesc.Height = args.m_Resolution.y;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = m_BackBufferFormat;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = m_4XMSAAState ? 4 : 1;
	sd.SampleDesc.Quality = m_4XMSAAState ? (m_4XMSAAQuality - 1) : 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = m_SwapChainBufferCount;
	sd.OutputWindow = args.m_WindowHandle;
	sd.Windowed = !args.m_Fullscreen;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT result = m_DxgiFactory2->CreateSwapChain(m_CommandQueue, &sd, &m_SwapChain);

	return true;
}

bool ZMD3DInterface::GetDisplayModeInfo(ZMInitArgs args)
{
	HRESULT result = S_OK;
	IDXGIFactory* dxgiFact = nullptr;
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&dxgiFact);
	RETURN_IF_FAILED(result);

	IDXGIAdapter* adapter = nullptr;
	result = dxgiFact->EnumAdapters(0, &adapter);
	RETURN_IF_FAILED(result);

	IDXGIOutput* adapterOutput = nullptr;
	result = adapter->EnumOutputs(0, &adapterOutput);
	RETURN_IF_FAILED(result);

	unsigned int numberOfModes = 0;
	result = adapterOutput->GetDisplayModeList(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED,
		&numberOfModes,
		NULL);//put modelist in here? we must trust the F�hrers instincts for now o.O

	if (numberOfModes == 0)
	{
		return false;
	}

	//contains info about the "modes" of the device, eg
	//width/height, refreshrate, scanlineordering(?) etc
	DXGI_MODE_DESC* modeList = new DXGI_MODE_DESC[numberOfModes];

	result = adapterOutput->GetDisplayModeList(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED,
		&numberOfModes,
		modeList);
	RETURN_IF_FAILED(result);

	for (int i = 0; i < numberOfModes; ++i)
	{
		if (modeList[i].Width == (unsigned int)args.m_Resolution.x &&
			modeList[i].Height == (unsigned int)args.m_Resolution.y)
		{
			m_Numerator = modeList[i].RefreshRate.Numerator;
			m_Denominator = modeList[i].RefreshRate.Denominator;
		}
	}

	DXGI_ADAPTER_DESC aDesc;
	ZeroMemory(&aDesc, sizeof(DXGI_ADAPTER_DESC));
	result = adapter->GetDesc(&aDesc);
	RETURN_IF_FAILED(result);

	m_VideoCardMemory = (int)(aDesc.DedicatedVideoMemory / 1024 / 1024);//mb


	//doesn't compile for some reason! 
	//unsigned long long str_length;
	//int error = 0;
	//error = wcstombs_s(
	//	str_length,
	//	m_VideoCardDesc,
	//	(size_t)128,
	//	aDesc.Description,
	//	(size_t)128);

	delete[] modeList;
	modeList = 0;

	adapterOutput->Release();
	adapterOutput = nullptr;

	adapter->Release();
	adapter = nullptr;

	dxgiFact->Release();
	dxgiFact = nullptr;

	return true;
}

bool ZMD3DInterface::CreateDevice(ZMInitArgs args)
{
#ifdef _DEBUG
	{
		ID3D12Debug* debugController;

		D3D12GetDebugInterface(IID_PPV_ARGS(&debugController));
		debugController->EnableDebugLayer();
	}
#endif // _DEBUG
	D3D_FEATURE_LEVEL feature_lvl = D3D_FEATURE_LEVEL_12_1;
	HRESULT result = D3D12CreateDevice(nullptr, feature_lvl, IID_PPV_ARGS(&m_Device));

	RETURN_IF_FAILED(result);
	
	return true;
}

bool ZMD3DInterface::CreateDescriptorHeap(ZMInitArgs args)
{
	ZeroMemory(&m_RtvHeapDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
	m_RtvHeapDesc.NumDescriptors = m_SwapChainBufferCount;
	m_RtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	m_RtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	m_RtvHeapDesc.NodeMask = 0;

	HRESULT result = m_Device->CreateDescriptorHeap(&m_RtvHeapDesc, IID_PPV_ARGS(&m_RenderTargetViewHeap));

	RETURN_IF_FAILED(result);

	
	ZeroMemory(&m_DsvHeapDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
	m_DsvHeapDesc.NumDescriptors = 1;
	m_DsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	m_DsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	m_DsvHeapDesc.NodeMask = 0;

	result = m_Device->CreateDescriptorHeap(&m_DsvHeapDesc, IID_PPV_ARGS(&m_DepthStencilViewHeap));
	RETURN_IF_FAILED(result);

	return true;
}

bool ZMD3DInterface::CreateRenderTargetView(ZMInitArgs args)
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(m_RenderTargetViewHeap->GetCPUDescriptorHandleForHeapStart());
	for (int i = 0; i < m_SwapChainBufferCount; i++)
	{
		m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&m_BackBufferRenderTarget[i]));

		m_Device->CreateRenderTargetView(m_BackBufferRenderTarget[i], nullptr, rtvHeapHandle);

		rtvHeapHandle.Offset(1, m_RtvDescriptorSize);
	}
	return true;
}

bool ZMD3DInterface::CreateDepthStencilTargetAndView(ZMInitArgs args)
{
	D3D12_RESOURCE_DESC depthStencilDesc;
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilDesc.Alignment = 0;
	depthStencilDesc.Width = args.m_Resolution.x;
	depthStencilDesc.Height = args.m_Resolution.y;
	depthStencilDesc.DepthOrArraySize = 1;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;// TODO: m_DepthStencilFormat;
	depthStencilDesc.SampleDesc.Count = m_4XMSAAState ? 4 : 1;
	depthStencilDesc.SampleDesc.Quality = m_4XMSAAState ? (m_4XMSAAQuality - 1) : 0;
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE optClear;
	optClear.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	optClear.DepthStencil.Depth = 1.f;
	optClear.DepthStencil.Stencil = 0;

	HRESULT result = m_Device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthStencilDesc,
		D3D12_RESOURCE_STATE_COMMON,
		&optClear,
		IID_PPV_ARGS(&m_DepthStencilBuffer));

	m_Device->CreateDepthStencilView(m_DepthStencilBuffer,
		nullptr,
		GetDepthStencilCPUHandle());

	m_CommandList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(m_DepthStencilBuffer,D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));


	return true;
}

bool ZMD3DInterface::CreateViewPort(ZMInitArgs args)
{
	D3D12_VIEWPORT vp;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = args.m_Resolution.x;
	vp.Height = args.m_Resolution.y;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.f;

	m_CommandList->RSSetViewports(1, &vp);

	m_ScissorRect = { 0,0,args.m_Resolution.x / 2,args.m_Resolution.y / 2 };
	m_CommandList->RSSetScissorRects(1, &m_ScissorRect);
	//////////// LEFT OFF HERE page 128
	return true;
}

bool ZMD3DInterface::CreateFence(ZMInitArgs args)
{
	HRESULT result = m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence));

	m_RtvDescriptorSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	m_DsvDescriptorSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	m_CbvSrvDescriptorSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	return true;
}

D3D12_CPU_DESCRIPTOR_HANDLE ZMD3DInterface::CurrentBackBufferView()const
{
	return GetBackBufferView(m_CurrentBackBuffer);
}

D3D12_CPU_DESCRIPTOR_HANDLE ZMD3DInterface::GetBackBufferView(UINT index)const
{
	if (index >= m_SwapChainBufferCount)
	{
		ASSERT(false, "Tried to access a buffer out of bounds!");
		return D3D12_CPU_DESCRIPTOR_HANDLE();
	}
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(
		m_RenderTargetViewHeap->GetCPUDescriptorHandleForHeapStart(),
		index,
		m_RtvDescriptorSize);// Correct?
}

D3D12_CPU_DESCRIPTOR_HANDLE ZMD3DInterface::GetDepthStencilCPUHandle()const
{
	return m_DepthStencilViewHeap->GetCPUDescriptorHandleForHeapStart();
}
