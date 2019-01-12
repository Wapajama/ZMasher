#pragma once
#include "BaseShader.h"
#include <ZMasherGfxDX11\TextureTypeEnums.h>
class ID3D11ShaderResourceView;
class ModelShader :
	public BaseShader
{
public:
	ModelShader();
	~ModelShader();

	virtual bool Apply(ID3D11DeviceContext* context)override;
	virtual bool Create(const wchar_t* source_file, ID3D11Device* device)override;

	inline void SetShaderResource(const eTextureType type, ID3D11ShaderResourceView* resource);

private:
	ID3D11ShaderResourceView* m_Textures[static_cast<unsigned char>(eTextureType::N_TEXTURES)];

};

inline void ModelShader::SetShaderResource(const eTextureType type, ID3D11ShaderResourceView* resource)
{
	m_Textures[static_cast<unsigned char>(type)] = resource;
}