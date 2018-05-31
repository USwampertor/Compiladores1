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
		PARSING_FLOAT,
		PARSING_CHAR,
		PARSING_RELATIONAL,
		PARSING_ARITHMETIC,
		PARSING_LOGICAL,
		PARSING_SEPARATOR,
		PARSING_DIMENSION,
		PARSING_GROUPING,
		PARSING_COMMENTARY
	};

		/*
		ID,
		INT,
		FLOAT,
		STRING,
		KEYWORD,
		RELATIONAL_OP,
		ARITHMETIC_OP,
		LOGICAL_OP,
		NEGATION_OP,
		SEPARATOR,
		ASIGN,
		DIMENSIONS,
		GROUPING
		*/
	public class Compiler_Lexicon
	{
	private:
		msclr::gcroot<Compiler_ErrorModule^> m_error;
		std::vector<Compiler_Token*> m_Tokens;
		std::map<std::string, std::string> m_KeyWordMap;
	public:
		Compiler_Lexicon(Compiler_ErrorModule^ errormodule);
		void AddToken(std::string lex, TOKEN_TYPE type, int line);
		void ClearToken();
		~Compiler_Lexicon();
		bool ParseSourceCode(const char* src);
		LEXIC_STATE m_LexState;
	};

}