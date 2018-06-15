#pragma once
#include "Compiler_Lexicon.h"
#include "Compiler_ErrorModule.h"
#include "Compiler_SintaxisStates.h"
#include "Compiler_SymbolsTable.h"
#include <stack>
using namespace System;
namespace CompilerCore
{
	public class Compiler_Sintax
	{
	private:
		Compiler_Lexicon * m_lexicMachine;
		Compiler_SymbolsTable* m_symbolsTable;
		msclr::gcroot<Compiler_ErrorModule^> m_error;

		std::stack<Compiler_SintaxisStates> m_stateStack();
	public:
		Compiler_Sintax(
			Compiler_Lexicon* lexicMachine, 
			Compiler_ErrorModule^ errorModule, 
			Compiler_SymbolsTable* table);
		~Compiler_Sintax();
		void SintaxStateMachine();
		void Clear();

	};
}

