// Compiler_Core.h
#pragma once
#include "windows.h"
#include "Compiler_ErrorModule.h"
#include "Compiler_SymbolsTable.h"
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
		//Syntax Class
		Compiler_Sintax* m_SintaxModule;
		//Semantic Class
		Compiler_Semantic* m_SemanticModule;


		//ErrorModule Class
		Compiler_ErrorModule^ m_ErrorModule;
		//SymbolsTable
		Compiler_SymbolsTable* m_SymbolsTable;
	};
}
