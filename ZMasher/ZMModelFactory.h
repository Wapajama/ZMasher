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
	ZMModel* LoadModel(ID3D11Device* device, const char* model_path);
	ZMModel* LoadFBXModel(ID3D11Device* device, const char* model_path);
	ZMModelInstanceNode* LoadModelInstance(ID3D11Device* device, const char * model_path);
protected:
	ZMModelFactory();
	friend class ZMSingleton<ZMModelFactory>;
private:

	ZMModelInstanceNode* InitModelInstanceNode(ZMModelNode* root_node, ZMModelInstanceNode* instance_node = nullptr);

	FbxManager* m_FbxManager;
	FbxScene* m_Scene;
	ZMModel* ProcessMesh(ID3D11Device* device, FbxNode* inNode);
	ZMModelNode* ProcessMeshHierarchy(ID3D11Device* device, FbxNode* inNode, ZMModelNode* parent = nullptr);

	//data orient this?
	GrowArray<ZMModelInstanceNode*> m_ModelInstances;
	GrowArray<ZMModel*> m_Models;
	GrowArray<ZMModelNode*> m_ModelNodes;

#pragma region OBJ
	void CountModelData(const char* model_path, int& vertex_pos_count, int& texture_count, int& normal_count, int& vertex_count);
	void ReadModelData(const char* model_path, float* vertex_pos_array, float* vertex_tex_array, unsigned long* indexes, CurrentVertexType* vertexes);
	bool ReadFace(std::ifstream& fin, int& vertex_count, const bool fourth_face = false);

	bool BruteForceAssertInixesTheSame();

	struct VertexID
	{
		short vertex_pos = 0;
		short texture = 0;
		short normal = 0;

		bool operator==(const VertexID& other)
		{
			return	vertex_pos == other.vertex_pos &&
				texture == other.texture &&
				normal == other.normal;
		}
	};

	bool DoesIDExist(const VertexID& id, int& index);

	GrowArray<unsigned long> m_Indexes;
	GrowArray<VertexID> m_VertexIDs;

	int m_NumberOfSquares;
	int m_Vertex_count;
#pragma endregion

};

