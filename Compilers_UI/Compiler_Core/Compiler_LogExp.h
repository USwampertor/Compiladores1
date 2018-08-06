#pragma once
#include "Compiler_TreeNode.h"
#include "Compiler_ExpTree.h"
#include "Compiler_Token.h"
#include <string.h>
#include <vector>
using namespace System;
namespace CompilerCore
{
	class Compiler_LogExp
	{
	private:
		int m_lineNumber;
		std::string m_funcName;
		std::string m_statement;
		std::vector<Compiler_Token*> m_infixExp;
		std::vector<Compiler_Token*> m_postfixExp;
		Compiler_ExpTree* m_tree;
		NODE_RESULT m_expectedResult;
		std::string m_symboltoUpdate;
		int m_symbolIndextoUpdate;

	public:
		Compiler_LogExp();
		~Compiler_LogExp();
	};
	
}

