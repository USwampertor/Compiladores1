#pragma once
#include "Compiler_Token.h"
namespace CompilerCore
{
	enum class NODE_RESULT
	{
		RESULT_INT,
		RESULT_FLOAT,
		RESULT_BOOL,
		RESULT_STRING
	};

	class Compiler_TreeNode
	{
	public:
		bool Evaluate();
		Compiler_TreeNode();
		Compiler_TreeNode(std::string type);
		void SetRightNode(Compiler_TreeNode* rgh);
		void SetLeftNode(Compiler_TreeNode* lft);
		~Compiler_TreeNode();
	private:
		Compiler_TreeNode* lnode;
		Compiler_TreeNode* rnode;
		NODE_RESULT m_nodeType;

	};
}

