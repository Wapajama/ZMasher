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
	
private:
	std::string* m_DataPath;
};
