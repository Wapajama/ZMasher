#pragma once
#include "BaseShader.h"
class ID3D11ShaderResourceView;
class ModelShader :
	public BaseShader
{
public:
	ModelShader();
	~ModelShader();

	virtual bool Apply(ID3D11DeviceContext* context)override;
	virtual bool Create(wchar_t* source_file, ID3D11Device* device)override;

	inline void SetShaderResource(ID3D11ShaderResourceView* resource);

private:
	ID3D11ShaderResourceView* m_Texture;

};

inline void ModelShader::SetShaderResource(ID3D11ShaderResourceView* resource)
{
	m_Texture = resource;
}