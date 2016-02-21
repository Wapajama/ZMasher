#include "ZMModelFactory.h"

#include <iostream>
#include <fstream>
#include <assert.h>

ZMModel* ZMModelFactory::LoadModel(ID3D11Device* device, const char * model_path)
{
	m_VertexIDs.clear();

	int vertex_pos_count = 0;
	m_Vertex_count = 0;
	int texture_count = 0;
	int normal_count = 0;
	int face_count = 0;
	int index_count = 0;

	CountModelData(model_path, vertex_pos_count, texture_count, normal_count, m_Vertex_count);

	//not using normals atm
	float* vertex_pos_array = new float[vertex_pos_count * 3];
	float* vertex_tex_array = new float[texture_count * 2];

	index_count = m_Indexes.size();
	CurrentVertexType* vertexes = new CurrentVertexType[m_Vertex_count];
	unsigned long* indexes = new unsigned long[index_count];

	ReadModelData(model_path, vertex_pos_array, vertex_tex_array, indexes, vertexes);

	ZMModel* model = new ZMModel();

	model->CreateModel(device, vertexes, indexes, m_Vertex_count, index_count);
	return model;
}

void ZMModelFactory::CountModelData(const char* model_path, int& vertex_pos_count, int& texture_count, int& normal_count, int& vertex_count)
{
	std::ifstream fin;

	fin.open(model_path);

	assert(fin.fail() == false);
	char input;
	fin.get(input);
	m_NumberOfSquares = 0;
	entire_file += input;
	while (fin.eof() == false)
	{
		if (input == 'v')
		{
			fin.get(input);
			entire_file += input;
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

		while (input == 'f')
		{
			fin.get(input);
			entire_file += input;
			if (input == ' ')
			{
				ReadFace(fin, vertex_count);
				ReadFace(fin, vertex_count);
				const bool test1 = ReadFace(fin, vertex_count);
				BruteForceAssertInixesTheSame();
				if (test1 == false)//false: 4 faces
				{
					ReadFace(fin, vertex_count, true);
					BruteForceAssertInixesTheSame();
					++m_NumberOfSquares;
				}
				fin.get(input);
				entire_file += input;
			}
			else
			{
				assert(false);
			}
		}
		while (input != '\n' &&
			   fin.eof() == false)
		{
			fin.get(input);
			entire_file += input;
		}
		fin.get(input);
		entire_file += input;
	}
}

bool ZMModelFactory::ReadFace(std::ifstream& fin, int& vertex_count, const bool fourth_face)
{
	char input = 0;

	short vertex_pos = 0;
	short texture = 0;
	short normal = 0;

	fin >> vertex_pos;
	entire_file += std::to_string(vertex_pos);
	fin.get(input);
	entire_file += input;
	fin >> texture;
	entire_file += std::to_string(texture);
	fin.get(input);
	entire_file += input;
	fin >> normal;
	entire_file += std::to_string(normal);
	fin.get(input);
	entire_file += input;

	assert(vertex_pos > 0 && texture > 0 && normal > 0);

	VertexID id;
	id.vertex_pos = vertex_pos - 1;
	id.texture = texture - 1;
	id.normal = normal - 1;

	int index;
	const bool copy_exists = DoesIDExist(id, index);

	if (copy_exists == false)
	{
		++vertex_count;
		index = m_VertexIDs.size();
		m_VertexIDs.push_back(id);
	}
	m_Indexes.push_back(index);
	if (fourth_face == true)
	{
		//the fourth vertex in a row. The three first creates a face,
		//fourth one requires work to be created
		m_Indexes.push_back(m_Indexes[m_Indexes.size() - 4]);
		m_Indexes.push_back(m_Indexes[m_Indexes.size() - 3]);
	}

	if (fin.eof() == false)
	{
		return input == '\n';
	}
	return true;
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
					fin >> vertex_pos_array[vertex_pos_index];
					fin >> vertex_pos_array[vertex_pos_index + 1];
					fin >> vertex_pos_array[vertex_pos_index + 2];
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
	for (int i = 0; i < m_Indexes.size(); ++i)
	{
		indexes[i] = m_Indexes[i];
	}
	for (int i = 0; i < m_Vertex_count; ++i)
	{
		int pos_index = m_VertexIDs[i].vertex_pos * 3;
		int tex_index = m_VertexIDs[i].texture * 2;
		vertexes[i].position.x = vertex_pos_array[pos_index];
		vertexes[i].position.y = vertex_pos_array[pos_index + 1];
		vertexes[i].position.z = vertex_pos_array[pos_index + 2];

		vertexes[i].tex.x = vertex_tex_array[tex_index];
		vertexes[i].tex.y = vertex_tex_array[tex_index + 1];
	}
}


bool ZMModelFactory::BruteForceAssertInixesTheSame()
{
	unsigned long indixes[3];
	indixes[0] = m_Indexes[m_Indexes.size() - 1];
	indixes[1] = m_Indexes[m_Indexes.size() - 2];
	indixes[2] = m_Indexes[m_Indexes.size() - 3];

	assert(m_Indexes.size() % 3 == 0);

	for (unsigned long i = 0; i < m_Indexes.size(); i += 3)
	{
		if (m_Indexes[i] == indixes[0] &&
			m_Indexes[i+1] == indixes[1] &&
			m_Indexes[i+2] == indixes[2])
		{
			return false;
		}
	}

	return true;
}