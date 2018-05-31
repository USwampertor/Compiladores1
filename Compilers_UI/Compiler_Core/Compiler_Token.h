#pragma once
#include <string>

namespace CompilerCore{

	public enum class TOKEN_TYPE
	{
		UNDEFINED = 0,
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
	};

	public class Compiler_Token
	{
	private:
		std::string m_Lexema;
		TOKEN_TYPE m_TokenType;
		int m_LineNumber;
	public:
		Compiler_Token(std::string lex, TOKEN_TYPE type, int line);
		std::string GetLex();
		TOKEN_TYPE GetType();
		~Compiler_Token();
	};

	
}


