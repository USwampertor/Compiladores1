// Archivo DLL principal.
#include "stdafx.h"
#include "Compiler_Core.h"
using namespace CompilerCore;
Manager::Manager()
{
	//Constructor
	m_ErrorModule = gcnew Compiler_ErrorModule();
	m_SymbolsTable = new Compiler_SymbolsTable();
	m_LexiconModule = new Compiler_Lexicon(m_ErrorModule);
	m_SemanticModule = new Compiler_Semantic(m_ErrorModule,m_SymbolsTable);
	m_SintaxModule = new Compiler_Sintax(m_LexiconModule, m_ErrorModule, m_SymbolsTable, m_SemanticModule);
}
Manager::~Manager()
{
	//Destroyer
	delete m_LexiconModule;
	delete m_SintaxModule;
	delete m_SymbolsTable;
	//lex = nullptr;
}
cli::array<String^>^ Manager::Compile(String^ src)
{
	//First we clear everything from our buffers, we don't want to get all the shit scrambled up from past compilings
	m_ErrorModule->Clear();
	m_LexiconModule->Clear();
	m_SintaxModule->Clear();
	//this is our entry to the DLL
	//src is referring to ALL the code the user put in our inputTextBox
	cli::array<String^>^ compilationDetails;
	//gcnew doesn't need a delete like new, if context dies, gcnew goes down with it
	///First Stage: Parser/Lexicon stage
	m_LexiconModule->ParseSourceCode((const char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(src).ToPointer());
	cli::array<String^>^ errorDetails = m_ErrorModule->ReturnErrorString();
	compilationDetails = gcnew cli::array<String^>(m_LexiconModule->GetNumTokens() + m_ErrorModule->ReturnNumberErrors()+1);
	for (int i = 0; i < m_ErrorModule->ReturnNumberErrors(); ++i)
	{
		compilationDetails[i] = errorDetails[i];
	}
	compilationDetails[m_ErrorModule->ReturnNumberErrors()] = "( ° w °)";
	for (int i = m_ErrorModule->ReturnNumberErrors()+1; i < (m_LexiconModule->GetNumTokens() + m_ErrorModule->ReturnNumberErrors()+1); ++i)
	{
		std::string a;
		const Compiler_Token* t = m_LexiconModule->GetNextToken();
		a = t->GetLex()+" "+t->GetType();
		compilationDetails[i] = gcnew String(a.c_str());
		//"token_name token_type"
	}
	m_SintaxModule->SintaxStateMachine();
	if(m_SymbolsTable->SymbolExists("alfa")) compilationDetails[m_ErrorModule->ReturnNumberErrors()] = "( u w u)";
	///Second Stage: Syntactic stage
	//
	///Third Stage: Semantic stage
	//
	///Finished everything up
	//
	///After compiling, you can return everything in the same string^, just separate the code from the tokens
	//You show the tokens in the debug
	return compilationDetails;
	

}