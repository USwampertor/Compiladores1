#pragma once
#include "windows.h"
#include "vcclr.h"
#include "Compiler_ErrorModule.h"
#include "Compiler_Token.h"
#include <vector>
#include <map>
#include <string>
using namespace System;

namespace CompilerCore {

	public enum class LEXIC_STATE
	{
		START,
		PARSING_ID,
		PARSING_INT,
		PARSING_CHAR,
		PARSING_FLOAT
	};

	
	public class Compiler_Lexicon
	{
	private:
		msclr::gcroot<Compiler_ErrorModule^> m_error;
		std::vector<Compiler_Token*> m_Tokens;
		std::map<std::string, std::string> m_KeyWordMap;
	public:
		Compiler_Lexicon(Compiler_ErrorModule^ errormodule);
		void AddToken(std::string lex, TOKEN_TYPE type, int line);
		~Compiler_Lexicon();
		bool ParseSourceCode(const char* src);
		LEXIC_STATE m_LexState;
	};

}