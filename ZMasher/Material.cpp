#include "Material.h"
#include <FX11\inc\d3dx11effect.h>

#define UCHAR_CAST(value)static_cast<unsigned char>(value)

Material::Material()
{
	for (unsigned char i = 0; i < UCHAR_CAST(eTextureType::N_TEXTURES); i++)
	{
		m_Textures[i] = nullptr;
	}
}

Material::~Material()
{
}

void Material::AddTexture(const eTextureType type, ID3D11ShaderResourceView* texture)
{
	m_Textures[UCHAR_CAST(type)] = texture;
}

ID3D11ShaderResourceView* Material::GetTexture(const eTextureType type)
{
	return m_Textures[UCHAR_CAST(type)];
}