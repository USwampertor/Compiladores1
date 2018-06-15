#pragma once

#include "Node.h"
using namespace System;
namespace CompilerCore 
{
	public class Compiler_GlobalNode : public Node
	{
		
	public:
		Compiler_GlobalNode();
		~Compiler_GlobalNode();
		void SetNode(Compiler_Token token)
		{
			m_nodeName = token.GetLex();
			m_eTokenType = token.GetType();

		}
	};
}


