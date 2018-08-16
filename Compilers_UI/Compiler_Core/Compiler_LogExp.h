#pragma once
#include "Compiler_TreeNode.h"
#include "Compiler_ExpTree.h"
#include "Compiler_Token.h"
#include <string.h>
#include <vector>
#include <stack>
#include "Compiler_ErrorModule.h"
using namespace System;

namespace CompilerCore
{
	
	public struct PolishStruct
	{
		
		PolishStruct() :operatorsin(0), operandsin(0) {};
		int operatorsin = 0, operandsin = 0;
		int separators = 0;
		int tokenit = 0;
		std::string m_functionProperty;
		std::string m_functionType;
		std::vector<const Compiler_Token*> /*tokensin,*/ outputTokens;
		//std::stack<std::vector<const Compiler_Token*>> tokensout;
		std::vector<const Compiler_Token*> original_vector;
		/*Add expression need to process two by one systems. Two operands by one operator
		While this rule doesn't apply, the system is faulty and wrote incorrectly*/
		bool Operator(const Compiler_Token* token)
		{
			if (
				token->GetLex() == "+" ||
				token->GetLex() == "-" ||
				token->GetLex() == "*" ||
				token->GetLex() == "/" ||
				token->GetLex() == "^" ||
				token->GetLex() == ">" ||
				token->GetLex() == "<")
			{
				return true;
			}
			return false;
		}
		bool Operand(const Compiler_Token* token)
		{
			if (
				token->GetType() == "ID" ||
				token->GetType() == "INT" ||
				token->GetType() == "FLOAT")
			{
				return true;
			}
			return false;
		}
		bool isExponential(const Compiler_Token* topToken)
		{
			if (topToken->GetLex() == "^")
			{
				return false;
			}
			return true;
		}
		bool HigherOperator(const Compiler_Token* topToken, const Compiler_Token* comparedToken)
		{
			int topweight = SetPriority(topToken); int compareweight = SetPriority(comparedToken);
			if (topweight == compareweight)
			{
				if (isExponential(topToken)) return false;
				else return true;
			}
			return topweight > compareweight ? true : false;
		}
		int SetPriority(const Compiler_Token* operandToken)
		{
			int priority = -1;
			if (operandToken->GetLex() == ">" || operandToken->GetLex() == "<")
			{
				priority = 1;
			}
			if (operandToken->GetLex() == "+" || operandToken->GetLex() == "-")
			{
				priority = 1;
			}
			if (operandToken->GetLex() == "*" || operandToken->GetLex() == "*")
			{
				priority = 2;
			}
			if (operandToken->GetLex() == "^")
			{
				priority = 3;
			}
			return priority;
		}
		bool IFtoPF(std::vector<const Compiler_Token*> operationVector)
		{
			std::vector<const Compiler_Token*> finalVector;
			std::stack<const Compiler_Token*> tokenStack;
			for (int i = 0; i < operationVector.size(); ++i)
			{
				if (Operator(operationVector[i]))
				{
					while (!tokenStack.empty() && tokenStack.top()->GetLex() != "(" && HigherOperator(tokenStack.top(), operationVector[i]))
					{
						finalVector.push_back(tokenStack.top());
						tokenStack.pop();
					}
					tokenStack.push(operationVector[i]);
				}
				else if (Operand(operationVector[i]))
				{
					finalVector.push_back(operationVector[i]);
				}
				else if (operationVector[i]->GetLex() == "(")
				{
					tokenStack.push(operationVector[i]);
				}
				else if (operationVector[i]->GetLex() == ")")
				{
					tokenStack.push(operationVector[i]);
				}
				else
				{
					return false;
				}
			}
			outputTokens = finalVector;
			return true;
		}
		bool AddExpression(Compiler_ErrorModule^ errorm, std::vector<const Compiler_Token*> operationVector)
		{
			operandsin = 0; operatorsin = 0;
			if (!ReverseNotation(operationVector, tokenit, errorm))
			{
				String^ strDesc = gcnew String("Invalid order in expression");
				String^ strLine = gcnew String(operationVector[tokenit]->GetLex().c_str());
				errorm->AddError(ERROR_PHASE::LEXICO, operationVector[tokenit]->GetLine(), strDesc, strLine);
				return false;
			}
			original_vector = operationVector;
			IFtoPF(operationVector);
			return true;
		}
		std::stack<std::vector<const Compiler_Token*>> GetTokensOut()
		{
			//return tokensout;
		}
		bool ReverseNotation(
			std::vector<const Compiler_Token*> tokenVector,
			int tokenat,
			Compiler_ErrorModule^ errorm)
		{
			//tokensin.clear();
			for (int i = tokenat; i < tokenVector.size(); ++i)
			{
				if (Operator(tokenVector[i]))
				{
					operatorsin++;
					//tokensin.push_back(tokenVector[i]);
				}
				else if (Operand(tokenVector[i]))
				{
					operandsin++;
					//tokensin.push_back(tokenVector[i]);
				}
				else if (tokenVector[i]->GetLex() == "(")
				{
					PolishStruct s;
					s.operandsin = operandsin;
					s.operatorsin = operatorsin;
					s.tokenit = tokenat;
					++separators;
					if (s.ReverseNotation(tokenVector, s.tokenit, errorm))
					{
						i = s.tokenit;
						operandsin = s.operandsin;
						operatorsin = s.operatorsin;
						//std::stack<std::vector<const Compiler_Token*>> t = s.GetTokensOut();
						//while (!t.empty())
						//{
						//	tokensout.push(t.top());
						//	t.pop();
						//}
					}
					else
					{
						String^ strDesc = gcnew String("Invalid order in expression");
						String^ strLine = gcnew String(tokenVector[i]->GetLex().c_str());
						errorm->AddError(ERROR_PHASE::LEXICO, tokenVector[i]->GetLine(), strDesc, strLine);
					}
				}
				else if (
					tokenVector[i]->GetLex() == ")"&&
					separators > 1)
				{
					tokenit = i;
					--separators;
					return true;
				}
				if (2 == operandsin && 1 == operatorsin)
				{
					//outputTokens.clear();
					//for (int j = 0; j < tokensin.size(); ++j)
					//{
					//	if (
					//		tokensin[j]->GetType() == "ID" ||
					//		tokensin[j]->GetType() == "INT" ||
					//		tokensin[j]->GetType() == "FLOAT")
					//	{
					//		outputTokens.push_back(tokensin[j]);
					//	}
					//}
					//for (int j = 0; j < tokensin.size(); ++j)
					//{
					//	if (tokensin[j]->GetType() == "ARITHMETIC OPERATOR")
					//	{
					//		outputTokens.push_back(tokensin[j]);
					//	}
					//}
					//tokensout.push(outputTokens);
					//operatorsin--; operandsin--;
				}
				else if ((2 > operandsin && 2 <= operatorsin))
				{
					tokenat = i;
					return false;
				}
			}
			return true;
		}
		void ClearPolish()
		{
			operatorsin = 0;
			operandsin = 0;
			separators = 0;
			tokenit = 0;
			//tokensin.clear();
			//outputTokens.clear();
			//while (!tokensout.empty())
			//{
			//	tokensout.pop();
			//}
		}
	};


	class Compiler_LogExp
	{
	private:
		int m_lineNumber;
		std::string m_funcName;
		std::string m_statement;
		Compiler_ExpTree* m_tree;
		NODE_RESULT m_expectedResult;
		std::string m_symboltoUpdate;
		int m_symbolIndextoUpdate;
		NODE_RESULT ThrowNodeResult(std::string functype);
	public:
		std::vector<const Compiler_Token*> m_infixExp;
		std::vector<const Compiler_Token*> m_postfixExp;
		Compiler_LogExp();
		Compiler_LogExp(PolishStruct pol);
		~Compiler_LogExp();
		void SetTree(Compiler_ExpTree* t);
	};
	
}

