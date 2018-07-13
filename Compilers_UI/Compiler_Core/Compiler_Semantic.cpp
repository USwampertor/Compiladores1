#include "stdafx.h"
#include "Compiler_Semantic.h"
using namespace CompilerCore;

Compiler_Semantic::Compiler_Semantic(Compiler_ErrorModule^ cterror, Compiler_SymbolsTable* cTable)
{

}

Compiler_Semantic::Compiler_Semantic()
{

}
Compiler_Semantic::~Compiler_Semantic()
{

}

void Compiler_Semantic::InFixToPostFix(std::vector<Compiler_Token*>)
{

}
void Compiler_Semantic::AddLogicalExp(std::vector<Compiler_Token*>)
{

}
std::string Compiler_Semantic::CreateStringKey(std::string funcname, int linenum)
{
	char* tmp; itoa(linenum, tmp, 10);
	return (funcname + "@" + tmp);
}
bool Compiler_Semantic::ValidateExpression()
{
	return true;
}

