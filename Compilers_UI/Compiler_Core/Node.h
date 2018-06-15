#pragma once
#include "Compiler_Token.h"
#include "Compiler_SymbolsTable.h"
#include <string.h>
#include <vector>
using namespace System;
namespace CompilerCore
{
	public class Node
	{
	public:
		NODE_TYPE m_NodeType;
		VAR_TYPE m_varType;
		int m_iArraySize = -1;
		std::string m_eTokenType;
		std::string m_nodeName;
		template <typename VARIABLE_TYPE>
		static std::vector<typename VARIABLE_TYPE> m_vInnerValues;
		Compiler_LocalNode* pLocalNode;
	public:
		Node();
		~Node();
		std::string GetName();
		std::string GetTokenType();
		int GetDimension();
		void SetLocalNode(Compiler_LocalNode& nextNode);
		template <typename VARIABLE_TYPE>
		void InsertValues(VARIABLE_TYPE[] values);
		virtual void SetNode(Compiler_Token token) = 0;
	};
}


