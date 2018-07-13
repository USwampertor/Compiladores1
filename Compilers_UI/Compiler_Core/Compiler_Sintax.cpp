#include "stdafx.h"
#include "Compiler_Sintax.h"
using namespace CompilerCore;

Compiler_Sintax::Compiler_Sintax(
	Compiler_Lexicon* lexicMachine, 
	Compiler_ErrorModule^ errorModule, 
	Compiler_SymbolsTable* table)
{
	m_error = errorModule;
	m_lexicMachine = lexicMachine;
	m_symbolsTable = table;
}

Compiler_Sintax::~Compiler_Sintax()
{
}

void Compiler_Sintax::SintaxStateMachine()
{
	Compiler_SintaxStates statemodule;
	m_lexicMachine->SetTokenIterator(0);
	statemodule.Run(m_lexicMachine, m_error, m_symbolsTable);
}
void Compiler_Sintax::Clear()
{
	m_symbolsTable->Clear();
}