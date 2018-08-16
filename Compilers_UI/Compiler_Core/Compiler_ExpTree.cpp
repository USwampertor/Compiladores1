#include "stdafx.h"
#include "Compiler_ExpTree.h"

using namespace CompilerCore;
Compiler_ExpTree::Compiler_ExpTree()
{
}
Compiler_ExpTree::~Compiler_ExpTree()
{
}
Compiler_ExpTree::Compiler_ExpTree(Compiler_TreeNode* r)
{
	root = r;
}
