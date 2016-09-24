#pragma once
#include <DataStructures\GrowArray.h>
#include <Math\ZMMatrix44.h>


class ZMModel;
class ZMModelNode
{
public:
	ZMModelNode(ZMModel* model);
	~ZMModelNode();

	inline ZMModel* GetModel();
	inline ZMModelNode* GetChild(const int index);

	inline void SetModel(ZMModel* model);
	inline void AddChild(ZMModelNode* child);
	inline const int ChildCount()const;

	inline ZMasher::Matrix44f GetTransform()const;
	inline void SetTransform(const ZMasher::Matrix44f& transform);

private:
	ZMModel* m_Model;
	GrowArray<ZMModelNode*> m_Children;
	ZMasher::Matrix44f m_Transform;//relative to parent, if it has any
};

inline ZMModel*				ZMModelNode::GetModel()
{
	return m_Model;
}
inline ZMModelNode*			ZMModelNode::GetChild(const int index)
{
	return m_Children[index];
}
inline void					ZMModelNode::SetModel(ZMModel* model)
{
	m_Model = model;
}
inline void					ZMModelNode::AddChild(ZMModelNode* child)
{
	m_Children.Add(child);
}
inline const int			ZMModelNode::ChildCount()const
{
	return m_Children.Size();
}
inline ZMasher::Matrix44f	ZMModelNode::GetTransform()const
{
	return m_Transform;
}
inline void					ZMModelNode::SetTransform(const ZMasher::Matrix44f& transform)
{
	m_Transform = transform;
}