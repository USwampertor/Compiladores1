// Compiler_Core.h
#pragma once
#include "windows.h"
#include "Compiler_ErrorModule.h"
#include "Compiler_Lexicon.h"
#include "Compiler_Sintax.h"
using namespace System;

namespace CompilerCore {
	public ref class Manager
	{
	public:
		Manager();
		~Manager();
		cli::array<String^>^ Compile(String^ src);
	private:
		//Lexicon Class
		Compiler_Lexicon* m_LexiconModule;
		//ErrorModule Class
		Compiler_ErrorModule^ m_ErrorModule;
		//Sintax Class
		Compiler_Sintax* m_SintaxModule;
	};
}
