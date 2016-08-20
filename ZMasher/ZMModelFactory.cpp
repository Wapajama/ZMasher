#include "ZMModelFactory.h"

#include <iostream>
#include <fstream>
#include <assert.h>
#include <fbxsdk.h>
#include <DirectXMath.h>
#include <vector>
#include <ZMUtils\Math\ZMVector4.h>
#include <ZMUtils\DataStructures\GrowArray.h>

#ifdef IOS_REF
	#undef  IOS_REF
	#define IOS_REF (*(pManager->GetIOSettings()))
#endif

void InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene)
{
	//The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
	pManager = FbxManager::Create();
	if (!pManager)
	{
		FBXSDK_printf("Error: Unable to create FBX Manager!\n");
		exit(1);
	}
	else FBXSDK_printf("Autodesk FBX SDK version %s\n", pManager->GetVersion());

	//Create an IOSettings object. This object holds all import/export settings.
	FbxIOSettings* ios = FbxIOSettings::Create(pManager, IOSROOT);
	pManager->SetIOSettings(ios);

	//Load plugins from the executable directory (optional)
	//FbxString lPath = FbxGetApplicationDirectory();
	//pManager->LoadPluginsDirectory(lPath.Buffer());

	//Create an FBX scene. This object holds most objects imported/exported from/to files.
	pScene = FbxScene::Create(pManager, "My Scene");
	if (!pScene)
	{
		FBXSDK_printf("Error: Unable to create FBX scene!\n");
		exit(1);
	}
}

bool LoadScene(FbxManager* pManager, FbxDocument* pScene, const char* pFilename)
{
	int lFileMajor, lFileMinor, lFileRevision;
	int lSDKMajor, lSDKMinor, lSDKRevision;
	//int lFileFormat = -1;
	int i, lAnimStackCount;
	bool lStatus;
	char lPassword[1024];

	// Get the file version number generate by the FBX SDK.
	FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

	// Create an importer.
	FbxImporter* lImporter = FbxImporter::Create(pManager, "");

	// Initialize the importer by providing a filename.
	const bool lImportStatus = lImporter->Initialize(pFilename, -1, pManager->GetIOSettings());
	lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

	if (!lImportStatus)
	{
		//note to self: "Unexpected file type" means "general error", it could be exactely anything
		//even that it is the incorrect path (why they simply don't say that is a mystery to me)
		FbxString error = lImporter->GetStatus().GetErrorString();
		FBXSDK_printf("Call to FbxImporter::Initialize() failed.\n");
		FBXSDK_printf("Error returned: %s\n\n", error.Buffer());

		if (lImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
		{
			FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
			FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);
		}

		return false;
	}

	FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);

	if (lImporter->IsFBX())
	{
		FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);

		// From this point, it is possible to access animation stack information without
		// the expense of loading the entire file.

		FBXSDK_printf("Animation Stack Information\n");

		lAnimStackCount = lImporter->GetAnimStackCount();

		FBXSDK_printf("    Number of Animation Stacks: %d\n", lAnimStackCount);
		FBXSDK_printf("    Current Animation Stack: \"%s\"\n", lImporter->GetActiveAnimStackName().Buffer());
		FBXSDK_printf("\n");

		for (i = 0; i < lAnimStackCount; i++)
		{
			FbxTakeInfo* lTakeInfo = lImporter->GetTakeInfo(i);

			FBXSDK_printf("    Animation Stack %d\n", i);
			FBXSDK_printf("         Name: \"%s\"\n", lTakeInfo->mName.Buffer());
			FBXSDK_printf("         Description: \"%s\"\n", lTakeInfo->mDescription.Buffer());

			// Change the value of the import name if the animation stack should be imported 
			// under a different name.
			FBXSDK_printf("         Import Name: \"%s\"\n", lTakeInfo->mImportName.Buffer());

			// Set the value of the import state to false if the animation stack should be not
			// be imported. 
			FBXSDK_printf("         Import State: %s\n", lTakeInfo->mSelect ? "true" : "false");
			FBXSDK_printf("\n");
		}

		// Set the import states. By default, the import states are always set to 
		// true. The code below shows how to change these states.
		IOS_REF.SetBoolProp(IMP_FBX_MATERIAL, true);
		IOS_REF.SetBoolProp(IMP_FBX_TEXTURE, true);
		IOS_REF.SetBoolProp(IMP_FBX_LINK, true);
		IOS_REF.SetBoolProp(IMP_FBX_SHAPE, true);
		IOS_REF.SetBoolProp(IMP_FBX_GOBO, true);
		IOS_REF.SetBoolProp(IMP_FBX_ANIMATION, true);
		IOS_REF.SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
	}

	// Import the scene.
	lStatus = lImporter->Import(pScene);

	if (lStatus == false && lImporter->GetStatus().GetCode() == FbxStatus::ePasswordError)
	{
		FBXSDK_printf("Please enter password: ");

		lPassword[0] = '\0';

		FBXSDK_CRT_SECURE_NO_WARNING_BEGIN
			scanf("%s", lPassword);
		FBXSDK_CRT_SECURE_NO_WARNING_END

			FbxString lString(lPassword);

		IOS_REF.SetStringProp(IMP_FBX_PASSWORD, lString);
		IOS_REF.SetBoolProp(IMP_FBX_PASSWORD_ENABLE, true);

		lStatus = lImporter->Import(pScene);

		if (lStatus == false && lImporter->GetStatus().GetCode() == FbxStatus::ePasswordError)
		{
			FBXSDK_printf("\nPassword is wrong, import aborted.\n");
		}
	}

	// Destroy the importer.
	lImporter->Destroy();

	return lStatus;
}

ZMModelFactory::ZMModelFactory() :m_Indexes(0), m_VertexIDs(0), m_Models(0)
{
	InitializeSdkObjects(m_FbxManager, m_Scene);
}

ZMModel* ZMModelFactory::LoadFBXModel(ID3D11Device* device, const char* model_path)
{
	LoadScene(m_FbxManager, m_Scene, model_path);
	return ProcessMesh(device, m_Scene->GetRootNode());
}

ZMModel* ZMModelFactory::ProcessMesh(ID3D11Device* device, FbxNode* inNode)
{
	FbxMesh* mesh = nullptr;
	for (short i = 0; i < inNode->GetChildCount(); ++i)
	{
		if (inNode->GetChild(i)->GetMesh() != nullptr)
		{
			mesh = inNode->GetChild(i)->GetMesh();
			break;
		}
	}
	//this fbx doesn't contain a mesh for some reason :s
	if (mesh == nullptr)
	{
		return nullptr;
	}

	int tri_count = mesh->GetPolygonCount();
	
	std::vector<ZMasher::Vector4f> control_points;

	for (short i = 0; i < mesh->GetControlPointsCount(); i++)
	{
		ZMasher::Vector4f cp;
		cp.x = static_cast<float>(mesh->GetControlPointAt(i).mData[0]);
		cp.y = static_cast<float>(mesh->GetControlPointAt(i).mData[1]);
		cp.z = static_cast<float>(mesh->GetControlPointAt(i).mData[2]);
		cp.w = 0.f;
		control_points.push_back(cp);
	}

	//std::vector<CurrentVertexType> vertices;
	//vertices.reserve(vertex_count);
	//std::vector<unsigned long> indexes;
	//indexes.reserve(vertex_count * 3);

	int index_count = mesh->GetPolygonCount() * 3;
	int vertex_count = mesh->GetPolygonCount() * 3;

	unsigned long* indexes = new unsigned long[index_count];
	CurrentVertexType* vertexes = new CurrentVertexType[vertex_count];

	int vertex_counter = 0;
	for (short i = 0; i < tri_count; ++i)
	{
		for (short j = 0; j < 3; ++j)
		{
			int cp_index = mesh->GetPolygonVertex(i, j);
			ZMasher::Vector4f cp = control_points[cp_index];

			CurrentVertexType temp;
			temp.position.x = cp.x;
			temp.position.y = cp.y;
			temp.position.z = cp.z;

			vertexes[vertex_counter] = temp;
			indexes[vertex_counter] = vertex_counter;
			//vertices.push_back(temp);
			//indexes.push_back(vertex_counter);
			++vertex_counter;
		}
	}

	ZMModel* model = new ZMModel();
	//model->CreateModel(device, &vertices[0], &indexes[0], vertex_count, index_count);
	
	//something's fucky -.-
	if (model->CreateModel(device, vertexes, indexes, vertex_count, index_count) == false)
	{
		return nullptr;
	}
	return model;
}

ZMModel* ZMModelFactory::LoadModel(ID3D11Device* device, const char * model_path)
{
	m_VertexIDs.Resize(512, false);
	m_Indexes.Resize(512, false);
	int vertex_pos_count = 0;
	m_Vertex_count = 0;
	int texture_count = 0;
	int normal_count = 0;
	int index_count = 0;

	CountModelData(model_path, vertex_pos_count, texture_count, normal_count, m_Vertex_count);

	//not using normals atm
	float* vertex_pos_array = new float[vertex_pos_count * 3];
	float* vertex_tex_array = new float[texture_count * 2];

	index_count = m_Indexes.Size();
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

		while (input == 'f')
		{
			fin.get(input);
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
		}
		fin.get(input);
	}
}

bool ZMModelFactory::ReadFace(std::ifstream& fin, int& vertex_count, const bool fourth_face)
{
	char input = 0;

	short vertex_pos = 0;
	short texture = 0;
	short normal = 0;

	fin >> vertex_pos;
	fin.get(input);
	fin >> texture;
	fin.get(input);
	fin >> normal;;
	fin.get(input);

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
		index = m_VertexIDs.Size();
		m_VertexIDs.Add(id);
	}
	m_Indexes.Add(index);
	if (fourth_face == true)
	{
		//the fourth vertex in a row. The three first creates a face,
		//fourth one requires work to be created
		m_Indexes.Add(m_Indexes[m_Indexes.Size() - 4]);
		m_Indexes.Add(m_Indexes[m_Indexes.Size() - 3]);
	}

	if (fin.eof() == false)
	{
		return input == '\n';
	}
	return true;
}

bool ZMModelFactory::DoesIDExist(const VertexID& id, int& index)
{
	for (short i = 0; i < m_VertexIDs.Size(); i++)
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
	for (int i = 0; i < m_Indexes.Size(); ++i)
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
	indixes[0] = m_Indexes[m_Indexes.Size() - 1];
	indixes[1] = m_Indexes[m_Indexes.Size() - 2];
	indixes[2] = m_Indexes[m_Indexes.Size() - 3];

	assert(m_Indexes.Size() % 3 == 0);

	for (unsigned long i = 0; i < m_Indexes.Size(); i += 3)
	{
		if (m_Indexes[i] == indixes[0] &&
			m_Indexes[i + 1] == indixes[1] &&
			m_Indexes[i + 2] == indixes[2])
		{
			return false;
		}
	}

	return true;
}


