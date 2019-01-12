#pragma once
#include <DirectXMath.h>
#include <Math/ZMVector.h>
#include <Math\ZMMatrix44.h>
#include <DataStructures\GrowArray.h>


class ZMModel;
class ZMModelNode;

class ZMModelInstanceNode
{
public:
	ZMModelInstanceNode(void);
	~ZMModelInstanceNode(void);

	inline void SetTransform(const ZMasher::Matrix44f& transform);
	inline ZMasher::Matrix44f GetTransform()const;

	inline void SetPosition(const ZMasher::Vector3f& position);
	inline ZMasher::Vector3f GetPosition();

	inline ZMModelNode* GetModelNode();
	inline void SetModelNode(ZMModelNode* model_node);

	inline void AddModelInstanceNode(ZMModelInstanceNode* node);
	inline ZMModelInstanceNode* GetChild(const int index);
	__forceinline const int ChildCount()const;

	inline void MarkForDelete() { m_MarkedForDelete = true; }
	inline bool IsMarkedForDelete()const {return m_MarkedForDelete;};


private:
	ZMasher::Matrix44f m_Transform;
	bool m_MarkedForDelete;
	ZMModelNode* m_ModelNode;

	GrowArray<ZMModelInstanceNode*> m_Children;

};

inline void ZMModelInstanceNode::SetTransform(const ZMasher::Matrix44f& transform)
{
	m_Transform = transform;
}
inline ZMasher::Matrix44f	ZMModelInstanceNode::GetTransform()const
{
	return m_Transform;
}
inline ZMModelNode*			ZMModelInstanceNode::GetModelNode()
{
	return m_ModelNode;
}
inline void					ZMModelInstanceNode::SetModelNode(ZMModelNode* model_node)
{
	m_ModelNode = model_node;
}
inline void					ZMModelInstanceNode::SetPosition(const ZMasher::Vector3f& position)
{
	m_Transform.SetTranslation(ZMasher::Vector4f(position.x,
												 position.y,
												 position.z,
												 m_Transform.GetTranslation().w));//dont want to mess up the matrix multiplications
}
inline ZMasher::Vector3f	ZMModelInstanceNode::GetPosition()
{
	const ZMasher::Vector4f translation = m_Transform.GetTranslation();
	return ZMasher::Vector3f(translation.x,
							 translation.y,
							 translation.z);
}
inline void					ZMModelInstanceNode::AddModelInstanceNode(ZMModelInstanceNode* node)
{
	m_Children.Add(node);
}
inline ZMModelInstanceNode*	ZMModelInstanceNode::GetChild(const int index)
{
	return m_Children[index];
}
__forceinline const int		ZMModelInstanceNode::ChildCount()const
{
	return m_Children.Size();
}