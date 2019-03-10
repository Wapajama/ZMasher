#include "ModelShader.h"
#include <Windows.h>
#include "ZMVertexTypes.h"
#include <FX11\inc\d3dx11effect.h>

ModelShader::ModelShader()
{
	for (unsigned char i = 0; i < static_cast<unsigned char>(eTextureType::N_TEXTURES); ++i)
	{
		m_Textures[i] = nullptr;
	}
}


ModelShader::~ModelShader()
{
}

bool ModelShader::Apply(ID3D11DeviceContext* context)
{
	//centralize shader variable names?
	GetDX11Effect()->GetVariableByName("albedoTexture")->AsShaderResource()->SetResource(m_Textures[static_cast<unsigned char>(eTextureType::ALBEDO)]);
	if (GetDX11Effect()->GetVariableByName("skybox"))
	{
		GetDX11Effect()->GetVariableByName("skybox")->AsShaderResource()->SetResource(m_Textures[static_cast<unsigned char>(eTextureType::ALBEDO)]);
	}
	GetDX11Effect()->GetVariableByName("normalTexture")->AsShaderResource()->SetResource(m_Textures[static_cast<unsigned char>(eTextureType::NORMAL)]);
	GetDX11Effect()->GetVariableByName("ambientOcclusionTexture")->AsShaderResource()->SetResource(m_Textures[static_cast<unsigned char>(eTextureType::AMBIENT_OCCLUSION)]);
	GetDX11Effect()->GetVariableByName("roughnessTexture")->AsShaderResource()->SetResource(m_Textures[static_cast<unsigned char>(eTextureType::ROUGHNESS)]);
	GetDX11Effect()->GetVariableByName("substanceTexture")->AsShaderResource()->SetResource(m_Textures[static_cast<unsigned char>(eTextureType::SUBSTANCE)]);
	return BaseShader::Apply(context);
}

bool ModelShader::Create(const char* source_file, ID3D11Device* device)
{
	HRESULT infoResult = BaseShader::Create(source_file, device);

	RETURNF_IF_FAILED(infoResult);
}