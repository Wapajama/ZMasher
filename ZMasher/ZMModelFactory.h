#pragma once
#include <Utility/ZMSingleton.h>
#include "ZMModel.h"
#include <string>
#include <DataStructures\GrowArray.h>

class ZMModelInstance;
class ZMModel;
class ID3D11Device;
class FbxManager;
class FbxScene;

namespace std
{
	typedef ifstream;
}
class ZMModelFactory: public ZMSingleton<ZMModelFactory>
{
public:
	ZMModel* LoadModel(ID3D11Device* device, const char* model_path);
	ZMModel* LoadFBXModel(ID3D11Device* device, const char* model_path);
protected:
	ZMModelFactory();
	friend class ZMSingleton<ZMModelFactory>;
private:
	

	FbxManager* m_FbxManager;


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
	GrowArray<ZMModel*> m_Models;

	int m_NumberOfSquares;
	int m_Vertex_count;
#pragma endregion

};

