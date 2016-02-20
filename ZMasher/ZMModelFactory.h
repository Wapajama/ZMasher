#pragma once
#include "ZMSingleton.h"
#include <vector>
#include "ZMModel.h"
class ZMModelInstance;
class ZMModel;
class ID3D11Device;
namespace std
{
	typedef ifstream;
}
class ZMModelFactory: public ZMSingleton<ZMModelFactory>
{
public:

	ZMModel* LoadModel(ID3D11Device* device, const char* model_path);

private:

	void CountModelData(const char* model_path, int& vertex_pos_count, int& texture_count, int& normal_count, int& vertex_count);
	void ReadModelData(const char* model_path, float* vertex_pos_array, float* vertex_tex_array, unsigned long* indexes, CurrentVertexType* vertexes);
	bool ReadFace(std::ifstream& fin);


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


	std::vector<unsigned long> m_Indexes;
	std::vector<VertexID> m_VertexIDs;
	std::vector<ZMModel*> m_Models;
};

