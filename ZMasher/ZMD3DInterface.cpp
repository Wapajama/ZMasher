#include "ZMD3DInterface.h"

#define RETURN_IF_FAILED(hres) 	\
	if (FAILED(result))\
	{\
		return false;\
	}\

ZMD3DInterface::ZMD3DInterface()
{
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
}


ZMD3DInterface::~ZMD3DInterface()
{
}

bool ZMD3DInterface::Init(ZMInitArgs args)
{
	m_VSync = args.m_VSync;

	//if (this->CreateDevice(args) == false)
	//{
	//	return false;
	//}
	//if (this->CreateViewPort(args) == false)
	//{
	//	return false;
	//}
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
		NULL);//put modelist in here? we must trust the Führers instincts for now o.O

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

	for (int i =0; i < numberOfModes; ++i)
	{
		if (modeList[i].Width == (unsigned int)args.m_Resolution.x &&
			modeList[i].Height== (unsigned int)args.m_Resolution.y)
		{
			m_Numerator = modeList[i].RefreshRate.Numerator;
			m_Denominator = modeList[i].RefreshRate.Denominator;
		}
	}

	DXGI_ADAPTER_DESC aDesc;
	ZeroMemory(&aDesc, sizeof(DXGI_ADAPTER_DESC));
	result = adapter->GetDesc(&aDesc);
	RETURN_IF_FAILED(result);

	m_VideoCardMemory = (int)(aDesc.DedicatedVideoMemory/1024/1024);//mb


	//doesn't compile for some reason! 
	//unsigned long long str_length;
	//int error = 0;
	//error = wcstombs_s(
	//	str_length,
	//	m_VideoCardDesc,
	//	(size_t)128,
	//	aDesc.Description,
	//	(size_t)128);

	delete [] modeList;
	modeList = 0;

	adapterOutput->Release();
	adapterOutput = nullptr;

	adapter->Release();
	adapter = nullptr;

	dxgiFact->Release();
	dxgiFact = nullptr;


	return true;
}

bool ZMD3DInterface::CreateDeviceAndSwapChain(ZMInitArgs args)
{
	HRESULT result = S_OK;
	
	//Los factory, nothing complicated! DXGI = Direct X Graphics Interface
	DXGI_SWAP_CHAIN_DESC swap_chain_desc;

	bool success = InitSwapChain(args, swap_chain_desc);

	if (success == false)
	{
		return false;
	}

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	result = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		&featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swap_chain_desc,
		&m_SwapChain,
		&m_Device,
		NULL,
		&m_Context);

	RETURN_IF_FAILED(result);


	ID3D11Texture2D* back_buffer;
	result = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer);
	RETURN_IF_FAILED(result);

	result = m_Device->CreateRenderTargetView(back_buffer, NULL, &m_RenderTarget);
	RETURN_IF_FAILED(result);

	back_buffer->Release();
	m_Context->OMSetRenderTargets(1, &m_RenderTarget, NULL);

	return true;
}

bool ZMD3DInterface::CreateDepthStencil(ZMInitArgs args)
{

}

bool ZMD3DInterface::InitDevice(ZMInitArgs args)
{
	return true;
}

bool ZMD3DInterface::InitSwapChain(ZMInitArgs args, DXGI_SWAP_CHAIN_DESC& swap_chain_desc)
{
	
	ZeroMemory(&swap_chain_desc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swap_chain_desc.BufferCount = 1;
	swap_chain_desc.BufferDesc.Width  = args.m_Resolution.x;
	swap_chain_desc.BufferDesc.Height = args.m_Resolution.y;

	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	/*
		TODO: find out what this here is good for, 
		it has something to do with refreshrate
	*/
	if (m_VSync == true)
	{
		swap_chain_desc.BufferDesc.RefreshRate.Numerator = m_Numerator;
		swap_chain_desc.BufferDesc.RefreshRate.Numerator = m_Denominator;
	}
	else
	{
		swap_chain_desc.BufferDesc.RefreshRate.Numerator = 0;
		swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.OutputWindow = args.m_WindowHandle;
	//this turns off multisampling
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;

	//"scan line ordering?"
	swap_chain_desc.Windowed = args.m_Fullscreen == false;
	swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swap_chain_desc.Flags = 0;
	return true;
}

bool ZMD3DInterface::CreateViewPort(ZMInitArgs args)
{
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = args.m_Resolution.x;
	viewport.Height = args.m_Resolution.y;

	m_Context->RSSetViewports(1, &viewport);

	return true;
}

void ZMD3DInterface::BeginScene()
{

}

void ZMD3DInterface::EndScene()
{

}

bool ZMD3DInterface::Render()
{

	m_SwapChain->Present(0, 0);
	return true;
}

bool ZMD3DInterface::Clear()
{
	float clear_color[] = { 0.0, 0.2f, 0.4f, 1.0f };
	m_Context->ClearRenderTargetView(m_RenderTarget, clear_color);	
	return true;
}

void ZMD3DInterface::Release()
{
	// close and release all existing COM objects
	m_SwapChain->Release();
	m_Device->Release();
	m_Context->Release();
	m_RenderTarget->Release();
}