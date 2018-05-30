#include "stdafx.h"
#include "Compiler_Lexicon.h"

using namespace CompilerCore;
Compiler_Lexicon::Compiler_Lexicon(Compiler_ErrorModule^ errormodule)
{
	m_error = errormodule;
	/*
	var
	int
	float
	bool
	string
	function
	main
	if
	else
	while
	for
	switch
	default
	return
	case*/
}


Compiler_Lexicon::~Compiler_Lexicon()
{
}
bool Compiler_Lexicon::ParseSourceCode(const char* src)
{
	int iCurrentLine = -1;
	const  char* cCurrChar = src;
	const char* cCurrLine = src;
	std::string sTokenBuffer;
	m_LexState = LEXIC_STATE::START;
	while (cCurrChar != NULL)
	{
		switch (m_LexState)
		{
		case LEXIC_STATE::START:
			if (isalpha(*cCurrChar) || (*cCurrChar == '_'))
			{
				//If the actual char is either a character or a '_', that means that we are processing either a keyword or an id
				sTokenBuffer.clear();
				sTokenBuffer.append(cCurrChar, 1);
				m_LexState = LEXIC_STATE::PARSING_ID;
				cCurrChar++;
			}
			else if (isdigit(*cCurrChar))
			{
				//If the actual char is a digit, we are processing a number
				sTokenBuffer.clear();
				sTokenBuffer.append(cCurrChar, 1);
				m_LexState = LEXIC_STATE::PARSING_INT;
				cCurrChar++;
			}
			else if ((*cCurrChar == '_'))
			{

			}
			break;
		case LEXIC_STATE::PARSING_ID:
			if (isalpha(*cCurrChar)||(*cCurrChar == '_')||isdigit(*cCurrChar))
			{
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
			}
			else
			{
				if (m_KeyWordMap.find(sTokenBuffer)!=m_KeyWordMap.end())
				{
					AddToken(sTokenBuffer, TOKEN_TYPE::KEYWORD, iCurrentLine);
				}
				else
				{
					AddToken(sTokenBuffer, TOKEN_TYPE::ID, iCurrentLine);
				}
			}
			break;
		case LEXIC_STATE::PARSING_INT:
			if (isdigit(*cCurrChar))
			{
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
			}
			if (*cCurrChar == '.')
			{
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
				LEXIC_STATE::PARSING_FLOAT;
			}
			else
			{
				AddToken(sTokenBuffer, TOKEN_TYPE::INT, iCurrentLine);
			}
			break;
		case LEXIC_STATE::PARSING_FLOAT:
			if (isdigit(*cCurrChar))
			{
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
			}
			else
			{
				AddToken(sTokenBuffer, TOKEN_TYPE::FLOAT, iCurrentLine);
			}
			break;
		}
	}
	return src;
}
void Compiler_Lexicon::AddToken(std::string lex, CompilerCore::TOKEN_TYPE type, int line)
{
	Compiler_Token* p = new Compiler_Token(lex, type, line);
	m_Tokens.push_back(p);
}