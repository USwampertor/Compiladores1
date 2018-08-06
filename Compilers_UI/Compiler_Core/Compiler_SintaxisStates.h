#pragma once
using namespace System;
#include <stack>
#include "Compiler_Token.h"
#include "Compiler_Lexicon.h"
#include "Compiler_ErrorModule.h"
#include "Compiler_SymbolsTable.h"
namespace CompilerCore
{	
	public struct SintaxBlob
	{
		int lineNum;
		int dimension;
		std::string symbol;
		std::string varType;
		std::string functionName;
	};
	
	//Struct to create the polish notation
	public struct PolishStruct
	{
		PolishStruct() :operatorsin(0), operandsin(0) {};

		int operatorsin = 0, operandsin = 0;
		int separators = 0;
		int tokenit = 0;
		std::vector<const Compiler_Token*> tokensin, outputTokens;
		std::stack<std::vector<const Compiler_Token*>> tokensout;
		std::vector<const Compiler_Token*> original_vector;
		/*Add expression need to process two by one systems. Two operands by one operator
		While this rule doesn't apply, the system is faulty and wrote incorrectly*/

		void AddExpression(Compiler_ErrorModule^ errorm, std::vector<const Compiler_Token*> operationVector)
		{
			operandsin = 0; operatorsin = 0;
			if (!ReverseNotation(operationVector,tokenit,errorm))
			{
				String^ strDesc = gcnew String("Invalid order in expression");
				String^ strLine = gcnew String(operationVector[tokenit]->GetLex().c_str());
				errorm->AddError(ERROR_PHASE::LEXICO, operationVector[tokenit]->GetLine(), strDesc, strLine);
			}
			original_vector = operationVector;
		}
		std::stack<std::vector<const Compiler_Token*>> GetTokensOut()
		{
			return tokensout;
		}
		bool ReverseNotation(
			std::vector<const Compiler_Token*> tokenVector, 
			int tokenat,
			Compiler_ErrorModule^ errorm)
		{
			tokensin.clear();
			for (int i = tokenat; i < tokenVector.size(); ++i)
			{
				if (tokenVector[i]->GetType()== "ARITHMETIC OPERATOR")
				{
					operatorsin++;
					tokensin.push_back(tokenVector[i]);
				}
				else if (
					tokenVector[i]->GetType() == "ID" ||
					tokenVector[i]->GetType() == "INT" ||
					tokenVector[i]->GetType() == "FLOAT")
				{
					operandsin++;
					tokensin.push_back(tokenVector[i]);
				}
				else if (tokenVector[i]->GetLex() == "(")
				{
					PolishStruct s;
					s.operandsin = operandsin;
					s.operatorsin = operatorsin;
					s.tokenit = tokenat;
					if (s.ReverseNotation(tokenVector,s.tokenit,errorm))
					{
						i = s.tokenit;
						operandsin = s.operandsin;
						operatorsin = s.operatorsin;
						std::stack<std::vector<const Compiler_Token*>> t = s.GetTokensOut();
						while (!t.empty())
						{
							tokensout.push(t.top());
							t.pop();
						}
					}
				}
				else if (
					tokenVector[i]->GetLex() == ")"&&
					separators > 1)
				{
					tokenit = i;
					return true;
				}
				if (2 == operandsin && 1 == operatorsin)
				{
					outputTokens.clear();
					for (int j = 0; j < tokensin.size(); ++j)
					{
						if (
							tokensin[j]->GetType() == "ID" ||
							tokensin[j]->GetType() == "INT" ||
							tokensin[j]->GetType() == "FLOAT")
						{
							outputTokens.push_back(tokensin[j]);
						}
					}
					for (int j = 0; j < tokensin.size(); ++j)
					{
						if (tokensin[j]->GetType() == "ARITHMETIC OPERATOR")
						{
							outputTokens.push_back(tokensin[j]);
						}
					}
					tokensout.push(outputTokens);
					operatorsin--; operandsin--;
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
			tokensin.clear();
			outputTokens.clear();
			while (!tokensout.empty())
			{
				tokensout.pop();
			}
		}
	};
	
	//Cluster class
	public class Compiler_SintaxStates
	{
		//Father state class
		class SintaxState
		{
		public:
			
			PolishStruct m_reverseStack;
			std::string m_funcname = "";
			std::vector<SintaxBlob> m_blobVector;
			std::stack<Compiler_Token*> m_tokenstack;
			const int m_tokenCounter = 0;
			bool instack;
			
			SintaxState() = default;
			
			~SintaxState() = default;
			
			void Release()
			{
				delete this;
			}
			
			virtual SintaxState* Process(
				Compiler_Lexicon* lexicMachine,
				Compiler_ErrorModule^ errorModule,
				Compiler_SymbolsTable* table,
				std::vector<PolishStruct> polish) = 0;
			
			virtual void RunInside() = 0;
			
			void AddBlob(const Compiler_Token* token = 0, std::string func = 0)
			{
				SintaxBlob blob;
				blob.symbol = token->GetLex();
				blob.varType = token->GetType();
				blob.lineNum = token->GetLine();
				blob.dimension = 0;
				blob.functionName = func;
				m_blobVector.push_back(blob);
			}
			
			void AddtoTable(Compiler_SymbolsTable* st)
			{
				for (int i = 0; i < m_blobVector.size(); ++i)
				{
					NODE_TYPE nodeType = NODE_TYPE::UNDEFINED;
					if (m_blobVector[i].functionName == "")
					{
						nodeType = NODE_TYPE::GLOBAL_NODE;
					}
					else
					{
						//if(this == std::unique_ptr<SintaxState_Block>)nodeType = NODE_TYPE::LOCAL_NODE;
						/*else*/ nodeType = NODE_TYPE::PARAMETER;
					}
					st->AddSymbol(
						m_blobVector[i].symbol, 
						nodeType,
						m_blobVector[i].dimension, 
						m_blobVector[i].varType, 
						m_blobVector[i].lineNum,
						m_blobVector[i].functionName);
				}
				m_blobVector.clear();
			}
			
			void AddSintaxError(Compiler_ErrorModule^ errorm ,const Compiler_Token* cToken, char* desc)
			{
				String^ strDesc = gcnew String(desc);
				String^ strLine = gcnew String(cToken->GetLex().c_str());
				errorm->AddError(ERROR_PHASE::LEXICO, cToken->GetLine(), strDesc, strLine);
			}
			
			void CheckStack(SintaxState* s)
			{
				//This function Checks the stat
			}
		};
		
		///////////////////////////////////////////////////////////////////////////////
		//Start state
		class SintaxState_Start : public SintaxState
		{
			void RunInside()
			{

			}
			
			SintaxState*  Process(
				Compiler_Lexicon* lexicMachine,
				Compiler_ErrorModule^ errorModule,
				Compiler_SymbolsTable* table,
				std::vector<PolishStruct> polish)
			{
				//VAR FUNCTION MAIN
				if (lexicMachine->GetNumTokens() == lexicMachine->GetTokenIterator()+1)
				{
					//We finished processing everything supposedly
					return nullptr;
				}
				if (lexicMachine->PeekTokenAt(lexicMachine->GetTokenIterator())->GetLex() == "var")
				{
					lexicMachine->GetNextToken();
					return new SintaxState_Var();
				}
				else if(lexicMachine->PeekTokenAt(lexicMachine->GetTokenIterator())->GetLex() == "function")
				{
					lexicMachine->GetNextToken();
					return new SintaxState_Function();
				}
				else if (lexicMachine->PeekTokenAt(lexicMachine->GetTokenIterator())->GetLex() == "main")
				{
					lexicMachine->GetNextToken();
					return new SintaxState_Block();
				}
				else
				{
					return nullptr;
				}
				return Process(lexicMachine,errorModule,table, polish);
			}
		};
		
		///////////////////////////////////////////////////////////////////////////////
		//Assign state
		class SintaxState_Assign : public SintaxState
		{
			void RunInside()
			{

			}
			SintaxState* Process(
				Compiler_Lexicon* lexicMachine,
				Compiler_ErrorModule^ errorModule,
				Compiler_SymbolsTable* table,
				std::vector<PolishStruct> polish)
			{

			}
		};
		
		///////////////////////////////////////////////////////////////////////////////
		//Assign state
		class SintaxState_Var : public SintaxState
		{
			void RunInside()
			{

			}
			SintaxState*  Process(
				Compiler_Lexicon* lexicMachine,
				Compiler_ErrorModule^ errorModule,
				Compiler_SymbolsTable* table,
				std::vector<PolishStruct> polish)
			{
				int i = 0;
				
					while (
						lexicMachine->GetActualToken()->GetLex() != ":"&&
						lexicMachine->GetActualToken()->GetLex() != ";")
					{
						if (lexicMachine->GetActualToken()->GetType() == "ID")
						{
							AddBlob(lexicMachine->GetActualToken(),m_funcname); ++i;
							//lexicMachine->GetNextToken();
							if (lexicMachine->PeekNextToken()->GetLex() == "[")
							{
								lexicMachine->GetNextToken();
								if (lexicMachine->PeekNextToken()->GetType() == "INT"&&
									atoi(lexicMachine->PeekNextToken()->GetLex().c_str()) > -1)
								{
									m_blobVector.back().dimension =
										atoi(lexicMachine->GetNextToken()->GetLex().c_str());
									if (lexicMachine->PeekNextToken()->GetLex() != "]")
									{
										//The [0] never closed, we need to send an error
										AddSintaxError(
											errorModule, 
											lexicMachine->GetNextToken(),
											"Array is never closed");
									}
									
								}
								else
								{
									//SEND ERROR THAT WE FOUND SOME [E] or some negative
									AddSintaxError(
										errorModule,
										lexicMachine->GetNextToken(),
										"Array value is invalid");
									while (lexicMachine->GetActualToken()->GetLex() != ";")
									{
										lexicMachine->GetNextToken();
									}
								}
							}
							else if (
								lexicMachine->PeekNextToken()->GetLex() == ",")
							{
								lexicMachine->GetNextToken();
							}
							else if (lexicMachine->PeekNextToken()->GetLex() == ":")
							{
								lexicMachine->GetNextToken();
								
							}

						}
						else
						{
							
							//most probably we found a var ,
							AddSintaxError(
								errorModule,
								lexicMachine->GetNextToken(),
								"Invalid argument");
							while (lexicMachine->GetActualToken()->GetLex() != ";")
							{
								lexicMachine->GetNextToken();
							}
							return nullptr;
						}
						
					} 
					if (lexicMachine->GetActualToken()->GetType() == "SEPARATOR" && i == 0)
					{
						//We send error because we have a var , or var : or var int etc
						AddSintaxError(
							errorModule,
							lexicMachine->GetNextToken(),
							"variable never declared");
						while (lexicMachine->GetNextToken()->GetLex() != ";")
						{
							lexicMachine->GetNextToken()->GetType();
						}
					}
					else
					{
						lexicMachine->GetNextToken();
						//At least we have som var a: int; and therefore we can dump our shit from here
						if (
							lexicMachine->GetActualToken()->GetLex() == "int"	||
							lexicMachine->GetActualToken()->GetLex() == "float" ||
							lexicMachine->GetActualToken()->GetLex() == "string" )
						{
							AddtoTable(table);
							lexicMachine->GetNextToken();
							if (lexicMachine->GetActualToken()->GetLex() != ";")
							{
								//they never closed with a ;
								AddSintaxError(
									errorModule,
									lexicMachine->GetActualToken(),
									"; missing at end of line");
							}
								//Shit went clean  and we actually had a var a,...n: int;
						}
						else
						{
							
							AddSintaxError(
								errorModule,
								lexicMachine->GetActualToken(),
								"Var type undefined");
							
						}
						return new SintaxState_Start();
					}
				
				return Process(lexicMachine, errorModule, table, polish);
			}
			public:
			SintaxState_Var() = default;
			SintaxState_Var(std::string func) { m_funcname = func; };
		};
		
		///////////////////////////////////////////////////////////////////////////////
		//Function Call state
		class SintaxState_FuncCall : public SintaxState
		{
			void RunInside()
			{

			}
			bool Run(
				Compiler_Lexicon* lexicMachine,
				Compiler_ErrorModule^ errorModule,
				Compiler_SymbolsTable* table)
			{

			}
			SintaxState*  Process(
				Compiler_Lexicon* lexicMachine,
				Compiler_ErrorModule^ errorModule,
				Compiler_SymbolsTable* table,
				std::vector<PolishStruct> polish)
			{

			}
		};
		
		///////////////////////////////////////////////////////////////////////////////
		//Function state
		class SintaxState_Function : public SintaxState
		{
			void RunInside()
			{
				//Here we are going to get all parameters needed for this function
				
			}
			bool Run(
				Compiler_Lexicon* lexicMachine,
				Compiler_ErrorModule^ errorModule,
				Compiler_SymbolsTable* table,
				std::vector<PolishStruct> polish)
			{
				SintaxState* p = new SintaxState_Parameter(m_funcname);
				p = p->Process(lexicMachine, errorModule, table, polish);
				if (p != nullptr)
				{
					return true;
				}
				return false;
			}
			SintaxState*  Process(
				Compiler_Lexicon* lexicMachine,
				Compiler_ErrorModule^ errorModule,
				Compiler_SymbolsTable* table,
				std::vector<PolishStruct> polish)
			{
				//return Process(lexicMachine, errorModule, table);
				if (lexicMachine->GetActualToken()->GetType()=="ID")
				{
					AddBlob(lexicMachine->GetActualToken(), m_funcname);
					m_funcname = lexicMachine->GetActualToken()->GetLex();

					if (lexicMachine->PeekNextToken()->GetLex() == "(")
					{
						lexicMachine->GetNextToken();
						if (!Run(lexicMachine, errorModule, table, polish))
						{
							AddSintaxError(
								errorModule,
								lexicMachine->GetNextToken(),
								"parameter not set correctly");
						}
						else
						{
							//AddtoTable(table);
							lexicMachine->GetNextToken();
							
							if (lexicMachine->GetActualToken()->GetLex() == ":")
							{
								//here we have to process the value it will return
								lexicMachine->GetNextToken();
								if (
									lexicMachine->GetActualToken()->GetLex() == "int" ||
									lexicMachine->GetActualToken()->GetLex() == "float" ||
									lexicMachine->GetActualToken()->GetLex() == "string"||
									lexicMachine->GetActualToken()->GetLex() == "keyword")
								{
									AddtoTable(table);
									//function has a return type
									lexicMachine->GetNextToken();
								if (lexicMachine->GetActualToken()->GetLex() == "{")
								{
									lexicMachine->GetNextToken();
									return new SintaxState_Block();
								}
								}
							}
							else
							{
									AddSintaxError(
									errorModule,
									lexicMachine->GetActualToken(),
									": and type missing at end of line");
							}
							return new SintaxState_Start();
						}
					}
				}
				else
				{
					//We send an error cause some bitch ass motherfucker just did a 
					//bamboozle here and put some shit like "function 3 ()"
					AddSintaxError(
						errorModule,
						lexicMachine->GetNextToken(),
						"variable never declared");
				}
				return nullptr;
			}
		};
		
		///////////////////////////////////////////////////////////////////////////////
		//Block state
		class SintaxState_Block : public SintaxState 
		{
			void RunInside()
			{

			}
			bool Run(Compiler_Lexicon* lexicMachine,
				Compiler_ErrorModule^ errorModule,
				Compiler_SymbolsTable* table,
				int runcase)
			{
				//if

				//while

				//func_call
				
				return false;
			}
			SintaxState* Process(
				Compiler_Lexicon* lexicMachine,
				Compiler_ErrorModule^ errorModule,
				Compiler_SymbolsTable* table,
				std::vector<PolishStruct> polish)
			{
				while (lexicMachine->GetActualToken()->GetLex() != "}")
				{
					if (lexicMachine->GetActualToken()->GetLex() == "if")
					{

						lexicMachine->GetNextToken();
						SintaxState* st = new SintaxState_If();
						st->Process(lexicMachine, errorModule, table, polish);
					}
					else if (lexicMachine->GetActualToken()->GetLex() == "for")
					{
						lexicMachine->GetNextToken();
						return new SintaxState_For();
					}
					else if (lexicMachine->GetActualToken()->GetLex() == "while")
					{
						lexicMachine->GetNextToken();
						return new SintaxState_While();
					}
					else if (
						lexicMachine->GetActualToken()->GetType() == "INT" ||
						lexicMachine->GetActualToken()->GetType() == "FLOAT" ||
						lexicMachine->GetActualToken()->GetType() == "ID")
					{

						const Compiler_Token* a = lexicMachine->GetActualToken();
						return  new SintaxState_LogicExp();
					}
					else
					{
						AddSintaxError(
							errorModule,
							lexicMachine->PeekTokenAt(lexicMachine->GetTokenIterator()),
							"Invalid character in block");
					}
				}
				
			}
		};
		
		///////////////////////////////////////////////////////////////////////////////
		//Parameter state
		class SintaxState_Parameter : public SintaxState
		{
		
			void RunInside()
			{

			}
			SintaxState* Process(
				Compiler_Lexicon* lexicMachine,
				Compiler_ErrorModule^ errorModule,
				Compiler_SymbolsTable* table,
				std::vector<PolishStruct> polish)
			{
				int i = 0;
				lexicMachine->GetNextToken();
				while (lexicMachine->GetActualToken()->GetLex() != ")")
				{
					
					if (lexicMachine->GetActualToken()->GetType() == "ID")
					{
						AddBlob(lexicMachine->GetActualToken(), m_funcname); ++i;
						//lexicMachine->GetNextToken();
						if (lexicMachine->PeekNextToken()->GetLex() == "[")
						{
							lexicMachine->GetNextToken();
							if (lexicMachine->PeekNextToken()->GetType() == "INT"&&
								atoi(lexicMachine->PeekNextToken()->GetLex().c_str()) > -1)
							{
								m_blobVector.back().dimension =
									atoi(lexicMachine->GetNextToken()->GetLex().c_str());
								if (lexicMachine->PeekNextToken()->GetLex() != "]")
								{
									//The [0] never closed, we need to send an error
									AddSintaxError(
										errorModule,
										lexicMachine->GetNextToken(),
										"Array is never closed");
								}

							}
							else
							{
								//SEND ERROR THAT WE FOUND SOME [E] or some negative
								AddSintaxError(
									errorModule,
									lexicMachine->GetNextToken(),
									"Array value is invalid");
								while (lexicMachine->GetActualToken()->GetLex() != ";")
								{
									lexicMachine->GetNextToken();
								}
							}
						}
						else if (
							lexicMachine->PeekNextToken()->GetLex() == ",")
						{
							lexicMachine->GetNextToken();
							lexicMachine->GetNextToken();

						}
						else if (lexicMachine->PeekNextToken()->GetLex() == ":")
						{
							lexicMachine->GetNextToken();
							lexicMachine->GetNextToken();

							//
							if (
								lexicMachine->GetActualToken()->GetLex() == "int" ||
								lexicMachine->GetActualToken()->GetLex() == "float" ||
								lexicMachine->GetActualToken()->GetLex() == "string")
							{
								AddtoTable(table);
								lexicMachine->GetNextToken();
								if (
									lexicMachine->GetActualToken()->GetLex() == ";"&&
									lexicMachine->PeekNextToken()->GetLex() == ")")
								{
									AddSintaxError(
										errorModule,
										lexicMachine->GetActualToken(),
										"SEPARATOR before GROUPING");
								}
								
							}
							else
							{
								
								AddSintaxError(
									errorModule,
									lexicMachine->GetActualToken(),
									"Didn't set var type in parameter");
							}
							
						}
					}
					else// if (lexicMachine->GetActualToken()->GetLex() == ":")
					{
						AddSintaxError(
							errorModule,
							lexicMachine->GetActualToken(),
							"Invalid variable inside parameters");
					}
				}
				
				if (lexicMachine->GetActualToken()->GetLex() == ")")
				{
					//Either the function foo () or function (a,b: int);
					return new SintaxState_Function();
				}
				else
				{
					// We found some dumb shit inside, don't know what we could find
					AddSintaxError(
						errorModule,
						lexicMachine->GetActualToken(),
						"the fuck u doing m8");
					return nullptr;
				}
				return Process(lexicMachine,errorModule,table, polish);
			}
		public:
			SintaxState_Parameter() = default;
			SintaxState_Parameter(std::string funcname) { m_funcname = funcname; }
		};

		///Haven't been implemented correctly
		
		///////////////////////////////////////////////////////////////////////////////
		//If state
		class SintaxState_If : public SintaxState
		{
			void RunInside()
			{

			}
			bool Run(
				Compiler_Lexicon* lexicMachine,
				Compiler_ErrorModule^ errorModule,
				Compiler_SymbolsTable* table,
				std::vector<PolishStruct> polish)
			{
				SintaxState * p = new SintaxState_LogicExp();
				p = p->Process(lexicMachine, errorModule, table, polish);
				if (p != nullptr)
				{
					return true;
				}
			}
				
			void RunInside(
				Compiler_Lexicon* lexicMachine,
				Compiler_ErrorModule^ errorModule,
				Compiler_SymbolsTable* table)
			{
				lexicMachine->GetNextToken();
				
			}
			SintaxState* Process(
				Compiler_Lexicon* lexicMachine,
				Compiler_ErrorModule^ errorModule,
				Compiler_SymbolsTable* table,
				std::vector<PolishStruct> polish)
			{
				if (lexicMachine->GetActualToken()->GetLex() == "(" ||
					lexicMachine->GetActualToken()->GetType() == "INT" ||
					lexicMachine->GetActualToken()->GetType() == "FLOAT" ||
					lexicMachine->GetActualToken()->GetType() == "ID")
				{
					if (Run(lexicMachine, errorModule, table, polish))
					{
						return new SintaxState_Start();
					}
				}
				if (lexicMachine->GetActualToken()->GetLex() == ")")
				{
					return new SintaxState_Start();
				}
				return Process(lexicMachine, errorModule, table, polish);
			}
		};

		///////////////////////////////////////////////////////////////////////////////
		//While state
		class SintaxState_While : public SintaxState 
		{
			void RunInside()
			{

			}
			SintaxState* Process(
				Compiler_Lexicon* lexicMachine,
				Compiler_ErrorModule^ errorModule,
				Compiler_SymbolsTable* table,
				std::vector<PolishStruct> polish)
			{
				return Process(lexicMachine, errorModule, table, polish);
			}
		};

		///////////////////////////////////////////////////////////////////////////////
		//For state
		class SintaxState_For : public SintaxState 
		{
			void RunInside()
			{

			}
			SintaxState* Process(
				Compiler_Lexicon* lexicMachine,
				Compiler_ErrorModule^ errorModule,
				Compiler_SymbolsTable* table,
				std::vector<PolishStruct> polish)
			{
				return Process(lexicMachine, errorModule, table, polish);
			}
		};
		
		///

		///////////////////////////////////////////////////////////////////////////////
		//Logic Expression state
		class SintaxState_LogicExp : public SintaxState
		{
			int separatorCounter = 0;
			int whereat = 0;
			int whereends = 0;
			std::vector<const Compiler_Token*> exptokens;
			void RunInside()
			{

			}
			SintaxState* Process(
				Compiler_Lexicon* lexicMachine,
				Compiler_ErrorModule^ errorModule,
				Compiler_SymbolsTable* table,
				std::vector<PolishStruct> polish)
			{
				whereat = lexicMachine->GetTokenIterator();
				
				do
				{
					if (lexicMachine->GetActualToken()->GetLex() == "(")
					{
						lexicMachine->GetNextToken();
						++separatorCounter;
					}
					else if (
						lexicMachine->GetActualToken()->GetType() == "INT" ||
						lexicMachine->GetActualToken()->GetType() == "FLOAT" ||
						lexicMachine->GetActualToken()->GetType() == "ID")
					{
						lexicMachine->GetNextToken();
						const Compiler_Token* a = lexicMachine->GetActualToken();
						if (lexicMachine->GetActualToken()->GetType() == "ARITHMETIC OPERATOR")
						{
							lexicMachine->GetNextToken();
						}
						else if (lexicMachine->GetActualToken()->GetLex() == ")"&&
							separatorCounter > 0)
						{
							lexicMachine->GetNextToken();
							--separatorCounter;
						}
						else if (lexicMachine->GetActualToken()->GetLex() == ";")
						{
							break;
						}
						else if (lexicMachine->GetActualToken()->GetLex() != "(")
						{
							AddSintaxError(
								errorModule,
								lexicMachine->GetActualToken(),
								"Invalid object inside expression");
								lexicMachine->GetNextToken();
						}
						
					}
					else if (lexicMachine->GetActualToken()->GetLex() == ";")
					{
						break;
					}
					else if (
						lexicMachine->GetActualToken()->GetLex() == ")"&& 
						separatorCounter <= 0)
					{
						AddSintaxError(
							errorModule,
							lexicMachine->GetActualToken(),
							"Using ) while theres no ( before");
						lexicMachine->GetNextToken();
					}
					else
					{
						AddSintaxError(
							errorModule,
							lexicMachine->GetActualToken(),
							"Invalid object in expression");
						lexicMachine->GetNextToken();
					}
					
					
				} while (separatorCounter == 0);
				if (lexicMachine->GetActualToken()->GetLex() == ";")
				{
					
					//It seems that we had everything correctly and now we can make the 
					//flush of data to the reverse notation
					whereends = lexicMachine->GetTokenIterator() - 1;
					for (int j = this->whereat; j < lexicMachine->GetNumTokens() && j <= this->whereends; ++j)
					{
						exptokens.push_back(lexicMachine->PeekTokenAt(j));
					}
					//This adds the polish struct into the vector of expressions
					m_reverseStack.AddExpression(errorModule, exptokens);
					polish.push_back(m_reverseStack);
					//Now that we have the info of the expression, we clear the struct so
					//We can keep using it for the next expression
					m_reverseStack.ClearPolish();
					return nullptr;
				}
				return Process(lexicMachine, errorModule, table, polish);
			}
		};


		//Member declaration for SintaxStates
	public:
		SintaxState* m_state;
		std::vector<PolishStruct> m_polacExpressions;
		std::stack<SintaxState*> m_sintaxstack;
		
		Compiler_SintaxStates()
		{
			m_state = new SintaxState_Start();
		}

		~Compiler_SintaxStates()
		{
			//delete m_state;
		}

		void Empty()
		{
			
			m_state->m_blobVector.clear();
			m_sintaxstack.empty();
		}
		
		void Run(
			Compiler_Lexicon* lexicMachine,
			Compiler_ErrorModule^ errorModule,
			Compiler_SymbolsTable* table)
		{
			do
			{
				m_state = m_state->Process(lexicMachine, errorModule, table, m_polacExpressions);
			} while (m_state != nullptr);
		}

	};

}


