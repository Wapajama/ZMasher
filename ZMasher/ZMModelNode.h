#pragma once
#include <DataStructures\GrowArray.h>
#include <Math\ZMMatrix44.h>


class ZMModel;
class ZMModelNode
{
public:
	ZMModelNode(ZMModel* model);
	~ZMModelNode();

	__forceinline ZMModel* GetModel();
	__forceinline ZMModelNode* GetChild(const int index);

	__forceinline void SetModel(ZMModel* model);
	__forceinline void AddChild(ZMModelNode* child);
	__forceinline const int ChildCount()const;

private:
	ZMModel* m_Model;
	GrowArray<ZMModelNode*> m_Children;
};

__forceinline ZMModel*		ZMModelNode::GetModel()
{
	return m_Model;
}
__forceinline ZMModelNode*	ZMModelNode::GetChild(const int index)
{
	return m_Children[index];
}
__forceinline void			ZMModelNode::SetModel(ZMModel* model)
{
	m_Model = model;
}
__forceinline void			ZMModelNode::AddChild(ZMModelNode* child)
{
	m_Children.Add(child);
}
__forceinline const int		ZMModelNode::ChildCount()const
{
	return m_Children.Size();
}
