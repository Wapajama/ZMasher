#include "ZMD3DInterface.h"

ZMD3DInterface::ZMD3DInterface()
{
}


ZMD3DInterface::~ZMD3DInterface()
{
}

bool ZMD3DInterface::Init(ZMInitArgs args)
{
	if (this->CreateDevice(args) == false)
	{
		return false;
	}
	if (this->CreateViewPort(args) == false)
	{
		return false;
	}
	return true;
}

bool ZMD3DInterface::CreateDevice(ZMInitArgs args)
{
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = args.m_WindowHandle;                                // the window to be used
	scd.SampleDesc.Count = 4;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode

															// create a device, device context and swap chain using the information in the scd struct
	D3D11CreateDeviceAndSwapChain(NULL,
								  D3D_DRIVER_TYPE_HARDWARE,
								  NULL,
								  NULL,
								  NULL,
								  NULL,
								  D3D11_SDK_VERSION,
								  &scd,
								  &m_SwapChain,
								  &m_Device,
								  NULL,
								  &m_Context);

	ID3D11Texture2D* back_buffer;
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer);

	m_Device->CreateRenderTargetView(back_buffer, NULL, &m_RenderTarget);
	back_buffer->Release();

	m_Context->OMSetRenderTargets(1, &m_RenderTarget, NULL);

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