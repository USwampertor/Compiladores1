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
	public class Compiler_Lexicon
	{
	private:
		msclr::gcroot<Compiler_ErrorModule^> m_error;
		std::vector<Compiler_Token*> m_Tokens;
		std::map<std::string,std::string> m_KeyWordMap;
		int tokeniterator;
	public:
		Compiler_Lexicon(Compiler_ErrorModule^ errormodule);
		~Compiler_Lexicon();
		int GetNumTokens();
		int GetTokenIterator();
		bool ParseSourceCode(const char* src);
		void Clear();
		void ClearToken();
		void ResetTokenCounter();
		void SetTokenIterator(int at);
		void AddToken(std::string lex, TOKEN_TYPE type, int line);
		void LexAddError(int line_num, char* desc, const char* line);
		const Compiler_Token* const GetNextToken();
		const Compiler_Token* const PeekTokenAt(int offset);
		const Compiler_Token* const GetActualToken();
		const Compiler_Token* const PeekNextToken();
		LEXIC_STATE m_LexState;
	};

}