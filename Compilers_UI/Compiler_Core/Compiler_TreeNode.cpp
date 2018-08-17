#include "stdafx.h"
#include "Compiler_TreeNode.h"

using namespace CompilerCore;
Compiler_TreeNode::Compiler_TreeNode()
{
}


Compiler_TreeNode::~Compiler_TreeNode()
{
}
Compiler_TreeNode::Compiler_TreeNode(std::string type)
{
	if (type == "string"|| type == "STRING")
	{
		m_nodeType = NODE_RESULT::RESULT_STRING;
	}
	if (type == "bool" || type == "BOOL")
	{
		m_nodeType = NODE_RESULT::RESULT_BOOL;
	}
	if (type == "int" || type == "INT")
	{
		m_nodeType = NODE_RESULT::RESULT_INT;
	}
	if (type == "float" || type == "FLOAT")
	{
		m_nodeType = NODE_RESULT::RESULT_FLOAT;
	}
}
bool Compiler_TreeNode::Evaluate()
{
	if ((lnode->m_nodeType == rnode->m_nodeType) && lnode == nullptr)
	{
		return true;
	}
	else if (lnode->m_nodeType == rnode->m_nodeType)
	{
		//m_nodeType = lnode->m_nodeType;
		return true;
	}
	else if(
		lnode->m_nodeType == NODE_RESULT::RESULT_INT   && rnode->m_nodeType == NODE_RESULT::RESULT_FLOAT ||
		lnode->m_nodeType == NODE_RESULT::RESULT_FLOAT && rnode->m_nodeType == NODE_RESULT::RESULT_INT   ||
		lnode->m_nodeType == NODE_RESULT::RESULT_FLOAT && rnode->m_nodeType == NODE_RESULT::RESULT_FLOAT  )

	{
		m_nodeType = NODE_RESULT::RESULT_FLOAT;
		return true;
	}
	return false;
	
}
void Compiler_TreeNode::SetRightNode(Compiler_TreeNode* rgh)
{
	rnode = rgh;
}
void Compiler_TreeNode::SetLeftNode(Compiler_TreeNode* lft)
{
	lnode = lft;
}
