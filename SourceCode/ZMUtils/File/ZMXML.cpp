#include "ZMXML.h"
#include <tinyxml2.h>
#include <Debugging\ZMDebugger.h>

#include <Math\Vector2.h>

ZMXML::ZMXML(const char* file)
{
	Init(file);
}

ZMXML::ZMXML(const std::string& file)
{
	Init(file.c_str());
}

ZMXML::~ZMXML()
{
	delete m_Doc;
}

void ZMXML::Init(const char* file)
{
	m_Doc = new tinyxml2::XMLDocument;

	if (file)
	{
		tinyxml2::XMLError error_code = m_Doc->LoadFile(file);
		if (error_code != tinyxml2::XMLError::XML_SUCCESS)
		{
			const std::string error_msg = tinyxml2::XMLDocument::ErrorIDToName(error_code);
			ASSERT(false, std::string("Failed to load: " + std::string(file) + ". Error message: " + error_msg).c_str());
		}
	}
}

const char* ZMXML::GetStringValue(const char* key)
{
	tinyxml2::XMLElement* root = m_Doc->RootElement();

	for(tinyxml2::XMLElement* it = root->FirstChildElement();
		it != nullptr;
		it = it->NextSiblingElement())
	{
		if (std::strcmp(it->Name(), key) == 0)
		{
			return it->Attribute("string", nullptr);
		}
	}
	return nullptr;
}

bool ZMXML::GetBoolValue(const char* key)
{
	tinyxml2::XMLElement* root = m_Doc->RootElement();

	for(tinyxml2::XMLElement* it = root->FirstChildElement();
		it != nullptr;
		it = it->NextSiblingElement())
	{
		if (std::strcmp(it->Name(), key) == 0)
		{
			return it->BoolAttribute("bool", false);
		}
	}
	return false;
}

int ZMXML::GetIntValue(const char* key)
{
	tinyxml2::XMLElement* root = m_Doc->RootElement();

	for (tinyxml2::XMLElement* it = root->FirstChildElement();
		it != nullptr;
		it = it->NextSiblingElement())
	{
		if (std::strcmp(it->Name(), key) == 0)
		{
			return it->IntAttribute("int", -INT_MAX);
		}
	}
	return -INT_MAX;
}

float ZMXML::GetFloatValue(const char* key)
{
	tinyxml2::XMLElement* root = m_Doc->RootElement();

	for (tinyxml2::XMLElement* it = root->FirstChildElement();
		it != nullptr;
		it = it->NextSiblingElement())
	{
		if (std::strcmp(it->Name(), key) == 0)
		{
			return it->FloatAttribute("float", -FLT_MAX);
		}
	}
	return -FLT_MAX;
}

ZMasher::Vector2<int> ZMXML::GetVec2IValue(const char* key)
{
	tinyxml2::XMLElement* root = m_Doc->RootElement();

	for (tinyxml2::XMLElement* it = root->FirstChildElement();
		it != nullptr;
		it = it->NextSiblingElement())
	{
		if (std::strcmp(it->Name(), key) == 0)
		{
			return ZMasher::Vector2i(it->IntAttribute("x"), it->IntAttribute("y"));
		}
	}
	return ZMasher::Vector2i(-INT_MAX, -INT_MAX);
}