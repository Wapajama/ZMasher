#include "ModelShader.h"
#include <d3dx11effect.h>
#include "ZMVertexTypes.h"

ModelShader::ModelShader()
{
}


ModelShader::~ModelShader()
{
}

bool ModelShader::Apply(ID3D11DeviceContext* context)
{
	GetDX11Effect()->GetVariableByName("shaderTexture")->AsShaderResource()->SetResource(m_Texture);
	return BaseShader::Apply(context);
}

bool ModelShader::Create(wchar_t* source_file, ID3D11Device* device)
{
	HRESULT infoResult = BaseShader::Create(source_file, device);

	RETURNF_IF_FAILED(infoResult);
}