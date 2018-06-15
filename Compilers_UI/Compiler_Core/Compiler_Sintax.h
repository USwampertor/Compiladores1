#pragma once
#include "Compiler_Lexicon.h"
using namespace System;
namespace CompilerCore
{
	public class Compiler_Sintax
	{
	public:
		Compiler_Sintax(Compiler_Lexicon lexMachine);
		~Compiler_Sintax();
		void CheckSintaxis();

	};
}

