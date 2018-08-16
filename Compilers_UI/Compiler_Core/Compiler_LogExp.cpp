#include "stdafx.h"
#include "Compiler_LogExp.h"

using namespace CompilerCore;

Compiler_LogExp::Compiler_LogExp()
{
}


Compiler_LogExp::~Compiler_LogExp()
{
}
Compiler_LogExp::Compiler_LogExp(PolishStruct pol)
{
	m_funcName = pol.m_functionProperty;
	m_infixExp = pol.original_vector;
	m_postfixExp = pol.outputTokens;
	m_expectedResult = ThrowNodeResult(pol.m_functionType);
}
NODE_RESULT Compiler_LogExp::ThrowNodeResult(std::string functype)
{
	if (functype == "boolean")
	{
		return NODE_RESULT::RESULT_BOOL;
	}
	if (functype == "float")
	{
		return NODE_RESULT::RESULT_FLOAT;
	}
	if (functype == "int")
	{
		return NODE_RESULT::RESULT_INT;
	}
}
void Compiler_LogExp::SetTree(Compiler_ExpTree* t)
{
	m_tree = t;
}


