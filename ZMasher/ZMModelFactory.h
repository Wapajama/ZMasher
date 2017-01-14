#pragma once
#include <Utility/ZMSingleton.h>
#include "ZMModel.h"
#include <string>
#include <DataStructures\GrowArray.h>
#include <fbxsdk.h>
#include <DataStructures\MemoryContainer.h>


class ZMModelInstance;
class ZMModel;
class ID3D11Device;
class ZMModelInstanceNode;
class ZMModelNode;
namespace std
{
	typedef ifstream;
}
class ZMModelFactory: public ZMSingleton<ZMModelFactory>
{
public:
	ZMModelInstanceNode* LoadModelInstance(const char * model_path);
	inline void SetDevice(ID3D11Device* device) { m_Device = device; }
protected:
	ZMModelFactory();
	friend class ZMSingleton<ZMModelFactory>;
private:

	ZMModelInstanceNode* InitModelInstanceNode(ZMModelNode* root_node, ZMModelInstanceNode* instance_node = nullptr);

	FbxManager* m_FbxManager;
	FbxScene* m_Scene;
	ZMModelNode* ProcessMeshHierarchy(FbxNode* inNode, ZMModelNode* parent = nullptr);

	//data orient this?
	GrowArray<ZMModelInstanceNode*> m_ModelInstances;
	GrowArray<ZMModel*> m_Models;
	GrowArray<ZMModelNode*> m_ModelNodes;
	ID3D11Device* m_Device;

};

