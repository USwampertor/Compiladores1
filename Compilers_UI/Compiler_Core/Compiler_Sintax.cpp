#include "stdafx.h"
#include "Compiler_Sintax.h"
using namespace CompilerCore;

Compiler_Sintax::Compiler_Sintax(
	Compiler_Lexicon* lexicMachine, 
	Compiler_ErrorModule^ errorModule, 
	Compiler_SymbolsTable* table,
	Compiler_Semantic* semantic)
{
	m_error = errorModule;
	m_lexicMachine = lexicMachine;
	m_symbolsTable = table;
	m_semanticMachine = semantic;
}

Compiler_Sintax::~Compiler_Sintax()
{
}

void Compiler_Sintax::SintaxStateMachine()
{
	Compiler_SintaxStates statemodule;
	m_lexicMachine->SetTokenIterator(0);
	statemodule.Run(m_lexicMachine, m_error, m_symbolsTable);
	if (!statemodule.m_polacExpressions.empty())
	{
		for (int i = 0; i < statemodule.m_polacExpressions.size(); ++i)
		{
			Compiler_LogExp*e = new Compiler_LogExp(statemodule.m_polacExpressions[i]);
			m_semanticMachine->m_notProcessedExpressions.push_back(e);
		}
	}
}
void Compiler_Sintax::Clear()
{
	m_symbolsTable->Clear();
}