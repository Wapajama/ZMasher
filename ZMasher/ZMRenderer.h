#pragma once
#include <vector> // TODO: create an in-house data structure for managing models
#include "ZMModel.h"
#include "ZMModelInstance.h"

class Camera;
class ZMD3DInterface;
class TextureShaderClass;

class ZMRenderer
{
public:
	ZMRenderer(void);
	~ZMRenderer(void);

	
	void Render(ZMD3DInterface& d3dinterface);

	void Init(ZMD3DInterface& d3dinterface);

	__forceinline void SetCamera(Camera* camera);

	/*
		This should later on be the lowest step in loading a model,
		since all the data orientation needs to take place here
		(or someplace similar)
	*/
	ZMModel* LoadModel(const char* filePath);

private:
	
	TextureShaderClass* m_TextureShader;//TODO: create a neat system for shaders
	Camera* m_Camera;
	std::vector<ZMModel> m_Models;
	std::vector<ZMModelInstance> m_ModelInstances;
};

__forceinline void ZMRenderer::SetCamera(Camera* camera)
{
	m_Camera = camera;
}