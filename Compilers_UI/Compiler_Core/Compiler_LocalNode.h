#pragma once
#include "Node.h"
#include <string>
using namespace System;
namespace CompilerCore
{
	public class Compiler_LocalNode: public Node
	{
	private:
		std::string context;
	public:
		Compiler_LocalNode();
		~Compiler_LocalNode();
		void SetNode(Compiler_Token token)
		{

		}
	};
}


