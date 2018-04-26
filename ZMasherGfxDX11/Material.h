#pragma once

#include <ZMasherGfxDX11/TextureTypeEnums.h>
#include <DataStructures\GrowArray.h>

class ID3D11ShaderResourceView;

class Material
{
public:
	Material();
	~Material();

	void AddTexture(const eTextureType type, ID3D11ShaderResourceView* texture);
	ID3D11ShaderResourceView* GetTexture(const eTextureType type);

private:

	ID3D11ShaderResourceView* m_Textures[(int)eTextureType::N_TEXTURES];
};

