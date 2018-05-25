#pragma once
using namespace System;

namespace CompilerCore {


	class Compiler_Lexicon
	{
	public:
		Compiler_Lexicon();
		~Compiler_Lexicon();
		bool ParseSourceCode(char* src);
	};

}