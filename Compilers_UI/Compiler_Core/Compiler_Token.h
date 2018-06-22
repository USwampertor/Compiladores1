#pragma once
#include <string>
#include <map>
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
		std::map<int, std::string> m_TokenMap;
	public:
		Compiler_Token(std::string lex, TOKEN_TYPE type, int line);
		const std::string GetLex() const;
		const std::string GetType() const;
		const int GetLine() const;
		~Compiler_Token();
	};

	
}


