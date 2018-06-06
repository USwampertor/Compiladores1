#include "stdafx.h"
#include "Compiler_Token.h"

using namespace CompilerCore;
Compiler_Token::Compiler_Token(std::string lex, TOKEN_TYPE type, int line)
{
	m_Lexema = lex;
	m_TokenType = type;
	m_LineNumber = line;
	m_TokenMap.insert(std::make_pair(0,"UNDEFINED"));
	m_TokenMap.insert(std::make_pair(1, "ID"));
	m_TokenMap.insert(std::make_pair(2,"INT"));
	m_TokenMap.insert(std::make_pair(3,"FLOAT"));
	m_TokenMap.insert(std::make_pair(4,"STRING"));
	m_TokenMap.insert(std::make_pair(5,"KEYWORD"));
	m_TokenMap.insert(std::make_pair(6,"RELATIONAL OPERATOR"));
	m_TokenMap.insert(std::make_pair(7,"ARITHMETIC OPERATOR"));
	m_TokenMap.insert(std::make_pair(8,"LOGICAL OPERATOR"));
	m_TokenMap.insert(std::make_pair(9,"NEGATION OPERATOR"));
	m_TokenMap.insert(std::make_pair(10,"SEPARATOR"));
	m_TokenMap.insert(std::make_pair(11,"ASIGN"));
	m_TokenMap.insert(std::make_pair(12,"DIMESIONS"));
	m_TokenMap.insert(std::make_pair(13,"GROUPING"));
}


Compiler_Token::~Compiler_Token()
{
}
const std::string Compiler_Token::GetLex() const
{
	return m_Lexema;
}
const std::string Compiler_Token::GetType() const
{
	return m_TokenMap.find((int)m_TokenType)->second;
}
