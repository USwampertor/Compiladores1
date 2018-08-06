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

void Compiler_Semantic::InFixToPostFix(std::vector<Compiler_Token*> inf, std::vector<Compiler_Token*> postf)
{

}
void Compiler_Semantic::AddLogicalExp(
	std::vector<Compiler_Token*> exp,
	int lineNum,
	std::string funcName,
	std::string statement,
	NODE_RESULT expectedRes,
	std::string symbol,
	int symbolIndex)
{
	///This is what i basically do in the SintaxStateMachine
	std::vector<Compiler_Token*> postfix;
	//InFixToPostFix(exp, postfix);
	Compiler_LogExp *le = new Compiler_LogExp();
	m_expVector.push_back(le);


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

