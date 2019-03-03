#pragma once

#include <string>

namespace tinyxml2
{
	class XMLDocument;
}
namespace ZMasher
{
	template<typename T>
	class Vector2;
}

class ZMXML
{
public:
	ZMXML(const char* file);
	ZMXML(const std::string& file);
	~ZMXML();

	const char* GetStringValue(const char* key);
	bool GetBoolValue(const char* key);
	int GetIntValue(const char* key);
	float GetFloatValue(const char* key);
	ZMasher::Vector2<int> GetVec2IValue(const char* key);
private:

	void Init(const char* file);

	tinyxml2::XMLDocument* m_Doc;


};

