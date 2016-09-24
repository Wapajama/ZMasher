#include "ZMModelNode.h"

ZMModelNode::ZMModelNode(ZMModel* model):
m_Model(model)
{
	m_Transform = ZMasher::Matrix44f::Identity();
}

ZMModelNode::~ZMModelNode()
{
}

