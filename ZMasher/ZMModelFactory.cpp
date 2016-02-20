#include "ZMModelFactory.h"

#include <iostream>
#include <fstream>
#include <assert.h>

ZMModel* ZMModelFactory::LoadModel(ID3D11Device* device, const char * model_path)
{
	m_VertexIDs.clear();

	int vertex_pos_count = 0;
	int vertex_count = 0;
	int texture_count = 0;
	int normal_count = 0;
	int face_count = 0;
	int index_count = 0;

	CountModelData(model_path, vertex_pos_count, texture_count, normal_count, vertex_count);

	//not using normals atm
	float* vertex_pos_array = new float[vertex_pos_count * 3];
	float* vertex_tex_array = new float[texture_count * 2];

	index_count = m_Indexes.size();
	CurrentVertexType* vertexes = new CurrentVertexType[vertex_count];
	unsigned long* indexes = new unsigned long[index_count];

	ReadModelData(model_path, vertex_pos_array, vertex_tex_array, indexes, vertexes);

	ZMModel* model = new ZMModel();

	model->CreateModel(device, vertexes, indexes, vertex_count, index_count);
	return model;
}

void ZMModelFactory::CountModelData(const char* model_path, int& vertex_pos_count, int& texture_count, int& normal_count, int& vertex_count)
{
	std::ifstream fin;

	fin.open(model_path);

	assert(fin.fail() == false);
	char input;
	fin.get(input);

	while (fin.eof() == false)
	{
		if (input == 'v')
		{
			fin.get(input);
			switch (input)
			{
				case ' ':
				{
					++vertex_pos_count;
					break;
				}
				case 't':
				{
					++texture_count;
					break;
				}
				case 'n':
				{
					++normal_count;
					break;
				}
			}
			continue;
		}

		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				while (input != '\n' &&
					   fin.eof() == false)
				{
					if (input == 's')
					{
						fin.get(input);
						fin.get(input);
						fin.get(input);
						continue;
					}
					if (ReadFace(fin) == true)++vertex_count;
					fin.get(input);//space
					if (ReadFace(fin) == true)++vertex_count;;
					fin.get(input);//space
					if(ReadFace(fin) == true)++vertex_count;
					
					fin.get(input);//newLine
				}
				
			}
		}
		while (input != '\n' &&
			   fin.eof() == false)
		{
			fin.get(input);
		}
		fin.get(input);
	}
}

bool ZMModelFactory::ReadFace(std::ifstream& fin)
{
	char input = 0;

	short vertex_pos = 0;
	short texture = 0;
	short normal = 0;

	fin >> vertex_pos;
	fin.get(input);
	fin >> texture;
	fin.get(input);
	fin >> normal;

	VertexID id;
	id.vertex_pos = vertex_pos - 1;
	id.texture = texture - 1;
	id.normal = normal - 1;

	int index;
	const bool copy_exists = DoesIDExist(id, index);

	if (copy_exists == false)
	{
		index = m_VertexIDs.size();
		m_VertexIDs.push_back(id);
	}
	m_Indexes.push_back(index);

	return copy_exists == false;//returns true if created a new one
}

bool ZMModelFactory::DoesIDExist(const VertexID& id, int& index)
{
	for (short i = 0; i < m_VertexIDs.size(); i++)
	{
		if (m_VertexIDs[i] == id)
		{
			index = i;
			return true;
		}
	}
	index = -1;
	return false;
}

void ZMModelFactory::ReadModelData(const char* model_path, float* vertex_pos_array, float* vertex_tex_array, unsigned long* indexes, CurrentVertexType* vertexes)
{
	int vertex_pos_index = 0;
	int vertex_index = 0;
	int texture_index = 0;
	int normal_index = 0;
	int face_index = 0;

	std::ifstream fin;
	fin.open(model_path);
	assert(fin.fail() == false);
	char input;
	fin.get(input);

	while (fin.eof() == false)
	{
		if (input == 'v')
		{
			fin.get(input);
			switch (input)
			{
				case ' ':
				{
					float test = -1;
					//fin >> vertex_pos_array[vertex_pos_index];
					//fin >> vertex_pos_array[vertex_pos_index + 1];
					//fin >> vertex_pos_array[vertex_pos_index + 2];
					fin >> test;
					vertex_pos_array[vertex_pos_index] = test;
					fin >> test;
					vertex_pos_array[vertex_pos_index + 1] = test;
					fin >> test;
					vertex_pos_array[vertex_pos_index + 2] = test;
					vertex_pos_index += 3;
					break;
				}
				case 't':
				{
					fin >> vertex_tex_array[texture_index];
					fin >> vertex_tex_array[texture_index + 1];
					texture_index += 2;
					break;
				}
				case 'n':
				{
					++normal_index;
					break;
				}
			}
			continue;
		}

		while (input != '\n' &&
			   fin.eof() == false)
		{
			fin.get(input);
		}
		fin.get(input);
	}
	for (short i = 0; i < m_Indexes.size(); i++)
	{
		indexes[i] = m_Indexes[i];
	}
	for (short i = 0; i < m_Indexes.size(); ++i)
	{
		const unsigned long current_index = m_Indexes[i];
		const int current_vertex_pos_index = m_VertexIDs[current_index].vertex_pos * 3;
		const int curret_tex_index = m_VertexIDs[current_index].texture * 2;
		vertexes[current_index].position.x = vertex_pos_array[current_vertex_pos_index];
		vertexes[current_index].position.y = vertex_pos_array[current_vertex_pos_index + 1];
		vertexes[current_index].position.z = vertex_pos_array[current_vertex_pos_index + 2];

		vertexes[current_index].tex.x = vertex_tex_array[curret_tex_index];
		vertexes[current_index].tex.y = vertex_tex_array[curret_tex_index + 1];
	}
}
