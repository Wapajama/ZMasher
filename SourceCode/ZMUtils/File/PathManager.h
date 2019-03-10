#pragma once
#include <ZMUtils\Utility\ZMSingleton.h>
#include <string>

class PathManager :
	public ZMSingleton<PathManager>
{
public:
	PathManager();
	~PathManager();

	std::string GetDataPath()const {return *m_DataPath;}
	std::string GetProjectRootPath()const {return *m_ProjectRootPath;}
	std::string GetShaderPath()const {return *m_ShaderPath;}
private:
	std::string* m_DataPath;
	std::string* m_ProjectRootPath;
	std::string* m_ShaderPath;
};