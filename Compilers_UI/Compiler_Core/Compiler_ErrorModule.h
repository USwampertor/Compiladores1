#pragma once

using namespace System;
namespace CompilerCore {

	public ref class Compiler_ErrorModule
	{
	public:
		Compiler_ErrorModule();
		~Compiler_ErrorModule();
		
		void AddError(ERROR_PHASE phase, int numLine, String^ sDesc, String^ sLine);
		int ReturnNumberErrors();
		cli::array<String^>^ ReturnErrorString();

	private:
		int m_MaxErrors, m_NumErrorsLex;
		cli::array<String^>^m_Error;
	};
	
	public enum class ERROR_PHASE
	{
		LEXICO,
		SINTACTICO,
		SEMANTICO
	};
}