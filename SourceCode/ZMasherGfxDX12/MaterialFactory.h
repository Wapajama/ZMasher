#pragma once

#include <DataStructures\GrowArray.h>

class MaterialFactory
{
public:
	MaterialFactory();
	~MaterialFactory();



private:
	struct MaterialSet
	{
		char m_ShaderType;
	};

};

