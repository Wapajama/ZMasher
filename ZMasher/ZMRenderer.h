#pragma once
#include <vector> // TODO: create an in-house data structure for managing models
#include "ZMModel.h"
#include "ZMModelInstanceNode.h"
#include <Math\ZMMatrix44.h>
#include <Profiler.h>

class Camera;
class ZMD3DInterface;
class TextureShaderClass;
class BaseShader;

class ZMRenderer
{
public:
	ZMRenderer(void);
	~ZMRenderer(void);

	void Render(ZMD3DInterface& d3dinterface, const float dt);

	void Init(ZMD3DInterface& d3dinterface);

	void RenderGrid(ZMD3DInterface& d3dinterface);

	inline void SetCamera(Camera* camera);

	/*
		This should later on be the lowest step in loading a model,
		since all the data orientation needs to take place here
		(or someplace similar)
	*/

private:

	//TODO: Let ZMRenderer merely be the "hub" of rendering interfaces, let
	//another class implement the render steps (renderblocks? posteffect helper?)

	void RenderModelHierarchy(ZMD3DInterface& d3dinterface, ZMModelInstanceNode* model, const ZMasher::Matrix44f& parent_orientation);
	void RenderSkybox(ZMD3DInterface& d3dinterface);
	void Render2DTerrain(ZMD3DInterface& d3dinterface);

	//TODO: create scene class that wraps all content specific data
	BaseShader* m_Shader; 
	BaseShader* m_SkyboxShader;
	BaseShader* m_TerrainShader;
	Camera* m_Camera;
	ZMModelInstanceNode* m_Skybox;
	ZMModelInstanceNode* m_Terrain;

#ifdef BENCHMARK
	ProfilerTaskID m_ModelsTimeStamp;
#endif // BENCHMMARK

	float m_Dt;
};

__forceinline void ZMRenderer::SetCamera(Camera* camera)
{
	m_Camera = camera;
}