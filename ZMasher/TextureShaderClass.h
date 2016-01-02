#pragma once

/*
	TODO: Make some other solution for these shader
	classes, perhaps a inheritance system or some 
	shader container? 2 things matter, API and 
	Data
*/

#include <d3d11.h>
#include <d3dx11effect.h>
#include <DirectXMath.h>

#include <fstream>

typedef WCHAR* FileType;

class TextureShaderClass
{
private:
	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

public:
	TextureShaderClass();
	TextureShaderClass(const TextureShaderClass&);
	~TextureShaderClass();

	bool Init(ID3D11Device*, HWND);
	void Shutdown();
	bool SetShaderVars(ID3D11DeviceContext*,
						const DirectX::XMMATRIX&, 
						const DirectX::XMMATRIX&, 
						const DirectX::XMMATRIX&, 
						ID3D11ShaderResourceView*);

private:
	bool InitShader(ID3D11Device*, HWND, FileType, FileType);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, 
								const DirectX::XMMATRIX&, 
								const DirectX::XMMATRIX&, 
								const DirectX::XMMATRIX&, 
								ID3D11ShaderResourceView*);

	void SetVariables(ID3D11DeviceContext*);

private:
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader*	m_PixelShader;
	ID3D11InputLayout*	m_Layout;
	ID3D11Buffer*		m_MatrixBuffer;

	ID3DX11Effect*		m_Effect;
	ID3DX11EffectTechnique* m_Technique;
};