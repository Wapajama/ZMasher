#include "ZMModelInstanceNode.h"


ZMModelInstanceNode::ZMModelInstanceNode(void)
{
	m_Children.Resize(4, false);
	m_Transform = ZMasher::Matrix44f::Identity();
}

ZMModelInstanceNode::~ZMModelInstanceNode(void)
{
}
	
