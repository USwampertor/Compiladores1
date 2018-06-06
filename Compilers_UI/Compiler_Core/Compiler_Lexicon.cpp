#include "stdafx.h"
#include "Compiler_Lexicon.h"
#define DQUOTES 34
#define NULLCHAR '\0';
using namespace CompilerCore;
Compiler_Lexicon::Compiler_Lexicon(Compiler_ErrorModule^ errormodule)
{
	m_error = errormodule;
	tokeniterator = -1;
	m_KeyWordMap.insert(std::make_pair("var", "var"));
	m_KeyWordMap.insert(std::make_pair("int", "int"));
	m_KeyWordMap.insert(std::make_pair("float", "float"));
	m_KeyWordMap.insert(std::make_pair("bool", "bool"));
	m_KeyWordMap.insert(std::make_pair("string", "string"));
	m_KeyWordMap.insert(std::make_pair("function", "function"));
	m_KeyWordMap.insert(std::make_pair("main", "main"));
	m_KeyWordMap.insert(std::make_pair("if", "if"));
	m_KeyWordMap.insert(std::make_pair("else", "else"));
	m_KeyWordMap.insert(std::make_pair("while", "while"));
	m_KeyWordMap.insert(std::make_pair("for", "for"));
	m_KeyWordMap.insert(std::make_pair("switch", "default"));
	m_KeyWordMap.insert(std::make_pair("default", "default"));
	m_KeyWordMap.insert(std::make_pair("return", "return"));
	m_KeyWordMap.insert(std::make_pair("case", "case"));

}


Compiler_Lexicon::~Compiler_Lexicon()
{
}
void Compiler_Lexicon::Clear()
{
	m_error->Clear();
	m_LexState=LEXIC_STATE::START;
	m_Tokens.clear();
	tokeniterator = -1;
}
bool Compiler_Lexicon::ParseSourceCode(const char* src)
{
	//Both bool variables help us to see a sub state of commentary and string states
	bool iscommentary = false, isstring = false;
	int ended = 2;
	int iCurrentLine = 1;
	const  char* cCurrChar = src;
	const char* cCurrLine = src;
	std::string sTokenBuffer;
	m_LexState = LEXIC_STATE::START;
	while (ended>0 && m_error->ReturnNumberErrors() <= m_error->ReturnMaxErrors())
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
			else if (*cCurrChar == '\r')
			{
				//We are doing a \r\n shit
				if (isstring)
				{
					//We made an enter before closing the string, so that's a no no and we throw an error
					LexAddError(iCurrentLine, "Changed lines before closing a string", cCurrLine);
				}
				else
				{
					//We found an ENTER, we change lines and let pass the fucker
					cCurrChar++;
					cCurrChar++;
					iCurrentLine++;
					cCurrLine = cCurrChar;
				}
			}
			else if (*cCurrChar == 32)
			{
				//Found a space, we let that bastard pass
				cCurrChar++;
			}
			else
			{
				//This shit is not processable, its an invalid character and therefore we found an error
				LexAddError(iCurrentLine, "Invalid character ", cCurrLine);
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
			else if (sTokenBuffer.back()=='.')
			{
				//We throw an error here because it means we have a 5.a or 5.# or some shit like that
				LexAddError(iCurrentLine, "Your float is not really completed m8", cCurrLine);
				m_LexState = LEXIC_STATE::START;
				
			}
			else
			{
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
				//That means we have either a <= >= == or !=
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
				AddToken(sTokenBuffer, TOKEN_TYPE::RELATIONAL_OP, iCurrentLine);
				m_LexState = LEXIC_STATE::START;
			}
			else if (*cCurrChar == sTokenBuffer.back())
			{
				//We either have a << or >> or !!, and that is a no no
				LexAddError(iCurrentLine, "Invalid relational operators m8", cCurrLine);
				m_LexState = LEXIC_STATE::START;
			}
			else
			{
				if (sTokenBuffer.back() == '=')
				{
					//We have a = and some other things next, therefore we can start processing the = and later work on the other object
					AddToken(sTokenBuffer, TOKEN_TYPE::ASIGN, iCurrentLine);
				}
				else if (sTokenBuffer.back() == '!')
				{
					//We have a ! and some other things next, therefore we can start processing the = and later work on the other object
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
			m_LexState = LEXIC_STATE::START;
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
					LexAddError(iCurrentLine, "Invalid AND operator syntax m8", cCurrLine);
				}
				else if (sTokenBuffer.back() == '|')
				{					
					//Throw error of invalid logical OR operator
					LexAddError(iCurrentLine, "Invalid OR operator syntax m8", cCurrLine);
				}
				m_LexState = LEXIC_STATE::START;
			}
			break;
		//Parsing CHARACTERS
		case LEXIC_STATE::PARSING_CHAR:
			//I'm using this state to process strings
			if (*cCurrChar != DQUOTES)
			{
				//We are just feeding the string beast until he's satisfied and is closed
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
			if (*cCurrChar != ',' || *cCurrChar != ':' || *cCurrChar != ';' && isalpha(*cCurrChar) || isdigit(*cCurrChar))
			{
				//Normally after this objects a char continues, so I'm taking that guess
				AddToken(sTokenBuffer, TOKEN_TYPE::SEPARATOR, iCurrentLine);
				m_LexState = LEXIC_STATE::START;
			}
			else
			{
				//This means we have something like ;, or ,: or :; etc. That shit's wack
				LexAddError(iCurrentLine, "you have a weird shit m8", cCurrLine);
			}
			break;

		}
		//Check if there's still something before closing the string
		if (*cCurrChar == '\0'&&sTokenBuffer.size() > 0)
		{
			--ended;
			/*TOKEN_TYPE token;
			SelectToken(m_LexState, token);
			AddToken(sTokenBuffer, token, iCurrentLine);*/
		}
		if (*cCurrChar == '\0'&& sTokenBuffer.size() == 0)
		{
			ended = 0;
		}

	} 
	if (isstring)
	{
		//We throw an error because the string was never closed
		LexAddError(iCurrentLine, "Never closed the string", cCurrLine);
	}
	if (iscommentary)
	{
		//We throw an error because comment was never closed
		LexAddError(iCurrentLine, "Never closed the comment", cCurrLine);
	}
	return m_error->ReturnNumberErrors() == 0;
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
void Compiler_Lexicon::LexAddError(int line_num, char* desc, const char* line)
{
	String^ strDesc = gcnew String(desc);
	String^ strLine = gcnew String(line);
	m_error->AddError(ERROR_PHASE::LEXICO, line_num, strDesc, strLine);
}
const Compiler_Token* const Compiler_Lexicon::GetNextToken()
{
	++tokeniterator;
	return m_Tokens[tokeniterator];
}
const Compiler_Token* const Compiler_Lexicon::PeekTokenAt(int offset)
{
	return m_Tokens[offset];
}
int Compiler_Lexicon::GetNumTokens()
{
	return m_Tokens.size();
}
void SelectToken(LEXIC_STATE state, CompilerCore::TOKEN_TYPE t)
{
	switch (state)
	{
	case CompilerCore::LEXIC_STATE::PARSING_ARITHMETIC:
		t = CompilerCore::TOKEN_TYPE::ARITHMETIC_OP;
		break;
	case CompilerCore::LEXIC_STATE::PARSING_CHAR:
		t = CompilerCore::TOKEN_TYPE::ARITHMETIC_OP;
		break;
	case CompilerCore::LEXIC_STATE::PARSING_COMMENTARY:
		break;
	case CompilerCore::LEXIC_STATE::PARSING_DIMENSION:
		break;
	case CompilerCore::LEXIC_STATE::PARSING_FLOAT:
		break;
	case CompilerCore::LEXIC_STATE::PARSING_GROUPING:
		break;
	case CompilerCore::LEXIC_STATE::PARSING_ID:
		break;
	case CompilerCore::LEXIC_STATE::PARSING_INT:
		break;
	case CompilerCore::LEXIC_STATE::PARSING_LOGICAL:
		break;
	case CompilerCore::LEXIC_STATE::PARSING_RELATIONAL:
		break;
	case CompilerCore::LEXIC_STATE::PARSING_SEPARATOR:
		break;
	}
}