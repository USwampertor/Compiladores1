#include "stdafx.h"
#include "Compiler_ErrorModule.h"

using namespace CompilerCore;

Compiler_ErrorModule::Compiler_ErrorModule()
{
	m_MaxErrors = 30;
	m_NumErrorsLex = 0;
	m_Error = gcnew cli::array<String ^>(m_MaxErrors);
}
Compiler_ErrorModule::~Compiler_ErrorModule()
{

}
void Compiler_ErrorModule::Clear()
{
	m_Error = gcnew cli::array<String ^>(m_MaxErrors);
	m_NumErrorsLex = 0;

}
void Compiler_ErrorModule::AddError(ERROR_PHASE phase, int numLine, String^ sDesc, String^ sLine)
{
	//We insert the error message here
	m_Error[m_NumErrorsLex] = (sDesc + "at line " + numLine + ": --->" + sLine);
	m_NumErrorsLex++;
}
int Compiler_ErrorModule::ReturnNumberErrors()
{
	return m_NumErrorsLex;
}
int Compiler_ErrorModule::ReturnMaxErrors()
{
	return m_MaxErrors;
}
cli::array<String^>^ Compiler_ErrorModule::ReturnErrorString()
{
	return m_Error;
}
