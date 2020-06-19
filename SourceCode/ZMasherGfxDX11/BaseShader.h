#pragma once
#include <DirectXMath.h>
#include <windows.h>
#include <project_defines.h>
#ifdef ZMASHER_DX12
class ID3D12Device;
class ID3D12ResourceView;
class ID3D11InputLayout;
class ID3D11Buffer;
class ID3DX11Effect;
class ID3D10Blob;
class ID3DX11EffectTechnique;
#else
class ID3D11Device;
class ID3D11DeviceContext;
class ID3D11ShaderResourceView;
class ID3D11InputLayout;
class ID3D11Buffer;
class ID3DX11Effect;
class ID3D10Blob;
class ID3DX11EffectTechnique;
#endif // ZMASHER_DX12




static void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const char* shaderFilename);

#define RETURNF_IF_FAILED(result) if (FAILED(result))\
	{\
		return false;\
	}\

struct __declspec(align(16)) MatrixBufferType
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

	virtual bool Create(const char* source_file, ID3D11Device* device);

	inline ID3DX11Effect* GetDX11Effect();
	inline ID3DX11EffectTechnique* GetDX11Technique();
	inline void SetVertexType(class VertexType* vert_type){m_VertexType = vert_type;}

private:

	VertexType* m_VertexType;

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