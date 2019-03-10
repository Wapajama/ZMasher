#include "PathManager.h"

PathManager::PathManager()
{
	m_ProjectRootPath = new std::string("../../../");
	m_DataPath = new std::string(*m_ProjectRootPath + "Data/");
	m_ShaderPath = new std::string(*m_ProjectRootPath + "Shaders/");
}


PathManager::~PathManager()
{
}
