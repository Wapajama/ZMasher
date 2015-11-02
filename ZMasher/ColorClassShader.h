#pragma once

/*
	For the sake of simplicity, this 
	here class is identical to the one 
	at rastertek.com. 

	TODO: Refactor this class 
	with consideraton of performance
	and codeflow in ZMasherMain
*/

#include <D3D11.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <fstream>

typedef  WCHAR* FileType;

class ColorClassShader
{
	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

public:
	ColorClassShader();
	ColorClassShader(const ColorClassShader& copy);
	~ColorClassShader();

	bool Init(ID3D11Device* device, HWND windowHandle);
	void ShutDown();
	bool Render(ID3D11DeviceContext* context, int,
				const DirectX::XMMATRIX& world,
				const DirectX::XMMATRIX& view,
				const DirectX::XMMATRIX& projection);
	bool InitShader(ID3D11Device* device, HWND windowHandle, FileType vsFileName, FileType psFileName);
	void ShutDownShader();
	void OutputShaderErrorMessage(ID3D10Blob* blob, HWND windowHandle, FileType);

	bool SetShaderParameters(ID3D11DeviceContext* context, 
							 const DirectX::XMMATRIX& world,
							 const DirectX::XMMATRIX& view,
							 const DirectX::XMMATRIX& projection);
	void RenderShader(ID3D11DeviceContext* context, int);

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_Layout;
	ID3D11Buffer* m_MatrixBuffer;

};

