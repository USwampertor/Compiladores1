#include "stdafx.h"
#include "Compiler_Lexicon.h"
#define DQUOTES 34
using namespace CompilerCore;
Compiler_Lexicon::Compiler_Lexicon(Compiler_ErrorModule^ errormodule)
{
	m_error = errormodule;
	/*
	Should we start the keywordmap with this keywords here?
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
	//Both bools help us to see a sub state of commentary and string states
	bool iscommentary = false, isstring = false;
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
				//We are processing either a keyword or an ID
				sTokenBuffer.clear();
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
				m_LexState = LEXIC_STATE::PARSING_ID;
			}
			else if (isdigit(*cCurrChar))
			{
				//We are processing numbers
				sTokenBuffer.clear();
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
				m_LexState = LEXIC_STATE::PARSING_INT;
			}
			else if (*cCurrChar == '<' || *cCurrChar == '>' || *cCurrChar == '!' || *cCurrChar == '=')
			{
				//We are processing a relational operator
				sTokenBuffer.clear();
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
				m_LexState = LEXIC_STATE::PARSING_RELATIONAL;
			}
			else if (*cCurrChar == '+' || *cCurrChar == '-' || *cCurrChar == '*' || *cCurrChar == '/' || *cCurrChar == '^' || *cCurrChar == '%')
			{
				//We are processing an arithmetic operator
				sTokenBuffer.clear();
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
				m_LexState = LEXIC_STATE::PARSING_ARITHMETIC;
			}
			else if (*cCurrChar == '(' || *cCurrChar == ')' || *cCurrChar == '{' || *cCurrChar == '}' || *cCurrChar == '[' || *cCurrChar == ']')
			{
				//We are processing an grouping operator
				sTokenBuffer.clear();
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
				m_LexState = LEXIC_STATE::PARSING_GROUPING;
			}
			else if (*cCurrChar == '&' || *cCurrChar == '|')
			{
				//We are processing either a && or a ||
				sTokenBuffer.clear();
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
				m_LexState = LEXIC_STATE::PARSING_LOGICAL;
			}
			else if (*cCurrChar == DQUOTES)
			{
				//We found a " and we are creating a string with it
				sTokenBuffer.clear();
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
				isstring = true;
				m_LexState = LEXIC_STATE::PARSING_CHAR;
			}
			else if (*cCurrChar == ':' || *cCurrChar == ',' || *cCurrChar == ';')
			{
				//We are processing a grouping operator
				sTokenBuffer.clear();
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
				m_LexState = LEXIC_STATE::PARSING_SEPARATOR;
			}
			else if (*cCurrChar == 9)
			{
				//We found a TAB, we just let pass the fucker
				cCurrChar++;
			}
			else if (*cCurrChar == 13)
			{
				//We found an ENTER, we change lines and let pass the fucker
				cCurrChar++;
				iCurrentLine++;
			}
			else
			{
				//This shit is not processable, its an invalid character and therefore we found an error
				//m_error->AddError();
			}
			break;
		//Parsing ID
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
				m_LexState = LEXIC_STATE::START;
			}
			break;
		//Parsing INT
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
				m_LexState = LEXIC_STATE::PARSING_FLOAT;
			}
			else
			{
				AddToken(sTokenBuffer, TOKEN_TYPE::INT, iCurrentLine);
				m_LexState = LEXIC_STATE::START;
			}
			break;
		//Parsing FLOAT
		case LEXIC_STATE::PARSING_FLOAT:
			if (isdigit(*cCurrChar))
			{
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
			}
			else
			{
				if (sTokenBuffer.back()=='.')
				{
					//We throw an error here because it means we have a 5.a or 5.# or some shit like that
					//m_error->AddError(ERROR_PHASE::LEXICO,iCurrentLine,"",cCurrLine);
				}
				AddToken(sTokenBuffer, TOKEN_TYPE::FLOAT, iCurrentLine);
				m_LexState = LEXIC_STATE::START;
			}
			break;
		//Parsing ARITHMETIC
		case LEXIC_STATE::PARSING_ARITHMETIC:
			if (*cCurrChar == '+' || *cCurrChar == '-' && sTokenBuffer.back() == *cCurrChar)
			{
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
				AddToken(sTokenBuffer, TOKEN_TYPE::ARITHMETIC_OP, iCurrentLine);
				m_LexState = LEXIC_STATE::START;
			}
			else if (*cCurrChar == '*' && sTokenBuffer.back() == '/' && iscommentary == false)
			{
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
				iscommentary = true;
				m_LexState = LEXIC_STATE::PARSING_COMMENTARY;
			}
			else
			{
				AddToken(sTokenBuffer, TOKEN_TYPE::ARITHMETIC_OP, iCurrentLine);
				m_LexState = LEXIC_STATE::START;
			}
			break;

		//Parsing RELATIONAL
		case LEXIC_STATE::PARSING_RELATIONAL:
			if (*cCurrChar == '=' && sTokenBuffer.back() == '>' || sTokenBuffer.back() == '<' || sTokenBuffer.back() == '=' || sTokenBuffer.back() == '!')
			{
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
				AddToken(sTokenBuffer, TOKEN_TYPE::RELATIONAL_OP, iCurrentLine);
				m_LexState = LEXIC_STATE::START;
			}
			else 
			{
				if (sTokenBuffer.back() == '=')
				{
					AddToken(sTokenBuffer, TOKEN_TYPE::ASIGN, iCurrentLine);
				}
				if (sTokenBuffer.back() == '!')
				{
					AddToken(sTokenBuffer, TOKEN_TYPE::NEGATION_OP, iCurrentLine);
				}
				else
				{
					AddToken(sTokenBuffer, TOKEN_TYPE::RELATIONAL_OP, iCurrentLine);
				}
				m_LexState = LEXIC_STATE::START;
			}
			break;
		//Parsing GROUPING
		case LEXIC_STATE::PARSING_GROUPING:
			break;
		//Parsing LOGICAL
		case LEXIC_STATE::PARSING_LOGICAL:
			if (*cCurrChar == sTokenBuffer.back())
			{
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
				AddToken(sTokenBuffer, TOKEN_TYPE::LOGICAL_OP, iCurrentLine);
				m_LexState = LEXIC_STATE::START;
			}
			else
			{
				if (sTokenBuffer.back() == '&')
				{
					//Throw error of invalid logical AND operator
				}
				else if (sTokenBuffer.back() == '|')
				{					
					//Throw error of invalid logical OR operator
				}
			}
			break;
		//Parsing CHARACTERS
		case LEXIC_STATE::PARSING_CHAR:
			//I'm using this state to process strings
			if (*cCurrChar != DQUOTES)
			{
				//We are just feeding the string beast until he's satisfied and closes itself
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
			}
			else if (*cCurrChar == DQUOTES && isstring == true)
			{
				//We are closing a string
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
				isstring = false;
				AddToken(sTokenBuffer, TOKEN_TYPE::STRING, iCurrentLine);
				m_LexState = LEXIC_STATE::START;
			}
			break;
		//Parsing COMMENTARY
		case LEXIC_STATE::PARSING_COMMENTARY:
			if (*cCurrChar == '/' && sTokenBuffer.back() == '*' && iscommentary == true)
			{
				//We are closing an open commentary
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
				iscommentary = false;
				AddToken(sTokenBuffer, TOKEN_TYPE::STRING, iCurrentLine);
				m_LexState = LEXIC_STATE::START;
			}
			else
			{
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
			}
			break;
		//Parsing SEPARATOR
		case LEXIC_STATE::PARSING_SEPARATOR:
			if (*cCurrChar != ',' || *cCurrChar != ':' || *cCurrChar != ';' && isalpha(*cCurrChar))
			{
				//Normally after this objects a char continues, so I'm taking that guess
				AddToken(sTokenBuffer, TOKEN_TYPE::SEPARATOR, iCurrentLine);
				m_LexState = LEXIC_STATE::START;
			}
			else
			{
				//This means we have something like ;, or ,: or :; etc. That shit's wack
				//m_error->AddError();
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
void Compiler_Lexicon::ClearToken()
{
	m_Tokens.clear();
}