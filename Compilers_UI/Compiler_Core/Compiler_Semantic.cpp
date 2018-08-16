#include "stdafx.h"
#include "Compiler_Semantic.h"
using namespace CompilerCore;

Compiler_Semantic::Compiler_Semantic(Compiler_ErrorModule^ cterror, Compiler_SymbolsTable* cTable)
{
	m_errorModule = cterror;
	m_symbolsTable = cTable;
}

Compiler_Semantic::Compiler_Semantic()
{

}
Compiler_Semantic::~Compiler_Semantic()
{

}
void Compiler_Semantic::InFixToPostFix(std::vector<const Compiler_Token*> inf, std::vector<const Compiler_Token*> postf)
{

}
void Compiler_Semantic::CreateExpressionTrees()
{
	PolishStruct comparator;
	std::stack<Compiler_TreeNode*> tree;
	for (int i = 0; i < m_notProcessedExpressions.size(); ++i)
	{
		//std::stack<const Compiler_Token*> tempStack;
		while (!tree.empty())
		{
			tree.pop();
		}
		for(int j = 0; j < m_notProcessedExpressions[i]->m_postfixExp.size(); ++j)
		{
			if (comparator.Operand(m_notProcessedExpressions[i]->m_postfixExp[j]))
			{
				Compiler_TreeNode* branch = new Compiler_TreeNode(m_notProcessedExpressions[i]->m_postfixExp[j]->GetType());
				tree.push(branch);
			}
			else if (comparator.Operator(m_notProcessedExpressions[i]->m_postfixExp[j]))
			{
				Compiler_TreeNode* branch = new Compiler_TreeNode(m_notProcessedExpressions[i]->m_postfixExp[j]->GetType());
				branch->SetRightNode(tree.top());
				tree.pop();
				branch->SetLeftNode(tree.top());
				if (branch->Evaluate())
				{
					tree.pop();
					tree.push(branch);
				}
				else
				{
					AddSemanticError(m_errorModule, m_notProcessedExpressions[i]->m_postfixExp[j], "Invalid Logic in expression");
					while (!tree.empty())
					{
						tree.pop();
					}
					break;
				}
			}
			
		}
		Compiler_ExpTree* expTree = new Compiler_ExpTree(tree.top());
		while (!tree.empty())
		{
			tree.pop();
		}
		m_notProcessedExpressions[i]->SetTree(expTree);
		m_expVector.push_back(m_notProcessedExpressions[i]);
	}
}
void Compiler_Semantic::AddLogicalExp(
	std::vector<const Compiler_Token*> exp,
	int lineNum,
	std::string funcName,
	std::string statement,
	NODE_RESULT expectedRes,
	std::string symbol,
	int symbolIndex)
{
	///This is what i basically do in the SintaxStateMachine
	//std::vector<Compiler_Token*> postfix;
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
void Compiler_Semantic::AddSemanticError(Compiler_ErrorModule^ errorm, const Compiler_Token* cToken, char* desc)
{
	String^ strDesc = gcnew String(desc);
	String^ strLine = gcnew String(cToken->GetLex().c_str());
	errorm->AddError(ERROR_PHASE::SEMANTICO, cToken->GetLine(), strDesc, strLine);
}
