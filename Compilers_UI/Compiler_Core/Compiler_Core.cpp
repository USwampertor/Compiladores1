// Archivo DLL principal.

#include "stdafx.h"
#include "Compiler_Core.h"
using namespace CompilerCore;
Manager::Manager()
{
	//Contructor
	m_LexiconModule = new Compiler_Lexicon();
	m_ErrorModule = gcnew Compiler_ErrorModule();
}
Manager::~Manager()
{
	//Destroyer
	delete m_LexiconModule;
	//lex = nullptr;
}
cli::array<String^>^ Manager::Compile(String^ src)
{
	//this is our entry to the DLL
	//src is refering to ALL the code the user put in our inputTextBox
	cli::array<String^>^ compilationDetails;
	compilationDetails = gcnew cli::array<String^>(1);
	compilationDetails[0] = gcnew String("Hola Mundo");
	//gcnew doesnt need a delete like new, if context dies, gcnew goes down with it

	///First Stage: Parser/Lexicon stage
	while ()
	{
	}

	//You have to sent the compiling errors around here
	//m_ErrorModule->AddError();

	//After compiling, you can return everything in the same string^, just separate the code from the tokens
	//you show the tokens in the debug
	return compilationDetails;
	

}