#include "stdafx.h"
#include "Compiler_Lexicon.h"

using namespace CompilerCore;
Compiler_Lexicon::Compiler_Lexicon(Compiler_ErrorModule^ errormodule)
{
	m_error = errormodule;
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
				sTokenBuffer.clear();
				sTokenBuffer.append(cCurrChar, 1);
				m_LexState = LEXIC_STATE::PARSING_ID;
				cCurrChar++;
			}
			else if (isdigit(*cCurrChar))
			{
				//
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
		}
	}
	return src;
}
void Compiler_Lexicon::AddToken(std::string lex, TOKEN_TYPE type, int line)
{
	Compiler_Token* p = new Compiler_Token(lex, type, line);
	m_Tokens.push_back(p);
}