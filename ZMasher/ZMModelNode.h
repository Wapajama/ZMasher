#pragma once
class ZMModel;

class ZMModelNode
{
public:
	ZMModelNode();
	~ZMModelNode();

private:
	ZMModel* m_Model;
	ZMModelNode* m_Children;
	short m_ChildCount;
};

