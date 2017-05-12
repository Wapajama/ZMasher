#pragma once
#include <DirectXMath.h>
#include <windows.h>

class ID3D11Device;
class ID3D11DeviceContext;
class ID3D11ShaderResourceView;
class ID3D11InputLayout;
class ID3D11Buffer;
class ID3DX11Effect;
class ID3D10Blob;
class ID3DX11EffectTechnique;

static void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, wchar_t* shaderFilename);

#define RETURNF_IF_FAILED(result) if (FAILED(result))\
	{\
		return false;\
	}\

struct MatrixBufferType
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
	DirectX::XMVECTOR cam_pos;
	float d_time;
};

class BaseShader
{
public:
	BaseShader();
	~BaseShader();

	virtual bool Apply(ID3D11DeviceContext* context) = 0;
	bool SetShaderVars(ID3D11DeviceContext*context,
					   const MatrixBufferType& constant_buffer);

	virtual bool Create(wchar_t* source_file, ID3D11Device* device);

	inline ID3DX11Effect* GetDX11Effect();
	inline ID3DX11EffectTechnique* GetDX11Technique();
private:

	ID3D11InputLayout*	m_Layout;
	ID3D11Buffer*		m_MatrixBuffer;

	ID3DX11Effect*		m_Effect;
	ID3DX11EffectTechnique* m_Technique;
};

inline ID3DX11Effect* BaseShader::GetDX11Effect()
{
	return m_Effect;
}
inline ID3DX11EffectTechnique* BaseShader::GetDX11Technique()
{
	return m_Technique;
}