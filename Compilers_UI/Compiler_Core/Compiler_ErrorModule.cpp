#include "stdafx.h"
#include "Compiler_ErrorModule.h"

using namespace CompilerCore;

Compiler_ErrorModule::Compiler_ErrorModule()
{
	m_MaxErrors = 15;
}
Compiler_ErrorModule::~Compiler_ErrorModule()
{

}
void Compiler_ErrorModule::AddError(ERROR_PHASE phase, int numLine, String^ sDesc, String^ sLine)
{
	m_NumErrorsLex++;
}
int Compiler_ErrorModule::ReturnNumberErrors()
{
	return 0;
}
cli::array<String^>^ Compiler_ErrorModule::ReturnErrorString()
{
	cli::array<String^>^ Errorstring;
	return Errorstring;
}
