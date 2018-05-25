// Compiler_Core.h

#pragma once
#include "Compiler_ErrorModule.h"
#include "Compiler_Lexicon.h"
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
		Compiler_Lexicon * m_LexiconModule;
		//ErrorModule Class
		Compiler_ErrorModule^ m_ErrorModule;
		// TODO: agregar aquí los métodos de la clase.
	};
}
