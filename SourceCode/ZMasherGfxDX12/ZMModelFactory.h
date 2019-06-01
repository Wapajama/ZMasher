#pragma once
#include <Utility/ZMSingleton.h>
#include <ZMasherGfxDX12/ZMModel.h>
#include <string>
#include <DataStructures\GrowArray.h>
#include <fbxsdk.h>
#include <DataStructures\MemoryContainer.h>
#include <ZMasherGfxDX12\TextureContainer.h>
#include <ZMasherGfxDX12\Material.h>
#include <ZMasherGfxDX12\dllHeader.h>

class ZMModelInstance;
class ZMModel;
class ID3D12Device;
class ZMModelInstanceNode;
class ZMModelNode;

namespace std
{
	typedef ifstream;
}

class ZMASHER_GFX_DX12_DLL ZMModelFactory: public ZMSingleton<ZMModelFactory>
{
	friend class ZMRenderer;//TODO: REMOVE
public:
	ZMModelFactory();
	ZMModelInstanceNode* LoadModelInstance(const char * model_path);
	ZMModelInstanceNode* LoadSkyBox(const char* skybox_path);//TODO: Create a neater implementation of this
	ZMModelInstanceNode* Load2DTerrain(const char* texture_path,
									   const float height = -5.f, 
									   const float width = 1000.f,
									   const float length = 1000.f);//TODO: same as above
	inline void SetDevice(ID3D12Device* device) { m_Device = device; m_TextureContainer.SetDevice(device); }

private:

	ZMModelInstanceNode* InitModelInstanceNode(ZMModelNode* root_node, ZMModelInstanceNode* instance_node = nullptr);

	FbxManager* m_FbxManager;
	FbxScene* m_Scene;
	ZMModelNode* ProcessMeshHierarchy(FbxNode* inNode, const std::string& model_path_str, ZMModelNode* parent = nullptr);

	ZMModel* AddModel();

	//data orient this?
	GrowArray<ZMModelInstanceNode*> m_ModelInstances;
	GrowArray<ZMModelNode*> m_ModelNodes;
	TextureContainer m_TextureContainer;
	GrowArray<Material> m_Materials;
	ID3D12Device* m_Device;

};