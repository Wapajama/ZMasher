#include "ZMModelInstanceNode.h"


ZMModelInstanceNode::ZMModelInstanceNode(void)
{
	m_Model = nullptr;
	m_Children.Resize(4, false);
}

ZMModelInstanceNode::~ZMModelInstanceNode(void)
{
}
	
