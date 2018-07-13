#pragma once
#include "Compiler_TreeNode.h"
namespace CompilerCore
{

	class Compiler_ExpTree
	{
	public:
		//vector de tokens del mapa en el constructor
		Compiler_ExpTree();
		~Compiler_ExpTree();
		bool Evalute();
	private:
		Compiler_TreeNode* root;
	};
}

