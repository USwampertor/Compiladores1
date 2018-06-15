#include "stdafx.h"
#include "Node.h"
using namespace CompilerCore;
Node::Node()
{
}


Node::~Node()
{
}
std::string	Node::GetName()
{
	return m_nodeName;
}
std::string Node::GetTokenType()
{
	return m_eTokenType;
}
int Node::GetDimension()
{
	return m_iArraySize;
}
void Node::SetLocalNode(Compiler_LocalNode& nextNode)
{
	*pLocalNode = nextNode;
}
template <typename VARIABLE_TYPE>
void Node::InsertValues(VARIABLE_TYPE[] values)
{
	for (int i=0; i<m_iArraySize; ++i)
	{
		m_vInnerValues[i] = values[];
	}
}