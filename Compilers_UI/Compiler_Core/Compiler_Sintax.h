#pragma once
#include "Compiler_Lexicon.h"
#include "Compiler_Semantic.h"
#include "Compiler_ErrorModule.h"
#include "Compiler_SymbolsTable.h"
#include "Compiler_SintaxisStates.h"
using namespace System;
namespace CompilerCore
{
	public class Compiler_Sintax
	{
	private:
		Compiler_Lexicon * m_lexicMachine;
		Compiler_SymbolsTable* m_symbolsTable;
		msclr::gcroot<Compiler_ErrorModule^> m_error;
		Compiler_Semantic * m_semanticMachine;
	public:
		Compiler_Sintax(
			Compiler_Lexicon* lexicMachine,
			Compiler_ErrorModule^ errorModule,
			Compiler_SymbolsTable* table,
			Compiler_Semantic* semantic);
		~Compiler_Sintax();
		void SintaxStateMachine();
		void Clear();

	};
}

