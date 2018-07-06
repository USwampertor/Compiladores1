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
		std::string symbol;
		int dimension;
		std::string varType;
		int lineNum;
		std::string functionName;
	};
	public class Compiler_SintaxStates
	{
		class SintaxState
		{
		public:
			static std::vector<SintaxBlob> m_blobVector;
			static std::stack<std::unique_ptr<SintaxState>> m_sintaxstack;
			const int m_tokenCounter = 0;
			SintaxState()
			{

			}
			~SintaxState()
			{

			}
			virtual std::unique_ptr<SintaxState> Process(
				Compiler_Lexicon* lexicMachine,
				Compiler_ErrorModule^ errorModule,
				Compiler_SymbolsTable* table) = 0;
			static void AddBlob(const Compiler_Token* token)
			{
				SintaxBlob blob;
				blob.symbol = token->GetLex();
				blob.varType = token->GetType();
				blob.lineNum = token->GetLine();
				blob.dimension = 0;
				blob.functionName = "";
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
			void CheckStack(std::unique_ptr<SintaxState> s)
			{
				//This function Checks the stat
			}
		};
		class SintaxState_Start : public SintaxState
		{
			std::unique_ptr<SintaxState>  Process(
				Compiler_Lexicon* lexicMachine,
				Compiler_ErrorModule^ errorModule,
				Compiler_SymbolsTable* table)
			{
				if (lexicMachine->PeekTokenAt(lexicMachine->GetTokenIterator())->GetLex() == "var")
				{
					lexicMachine->GetNextToken();
					std::unique_ptr<SintaxState> tmp;// (new SintaxState_Var);
					return tmp;
				}
				else if(lexicMachine->PeekTokenAt(lexicMachine->GetTokenIterator())->GetLex() == "function")
				{
					lexicMachine->GetNextToken();
					return std::unique_ptr<SintaxState_Function>();
				}
				else if (lexicMachine->PeekTokenAt(lexicMachine->GetTokenIterator())->GetLex() == "(")
				{
					lexicMachine->GetNextToken();
					return std::unique_ptr<SintaxState_Parameter>();
				}
				else
				{
					return nullptr;
				}
				return Process(lexicMachine,errorModule,table);
			}
		};
		class SintaxState_Var : public SintaxState
		{

			std::unique_ptr<SintaxState>  Process(
				Compiler_Lexicon* lexicMachine,
				Compiler_ErrorModule^ errorModule,
				Compiler_SymbolsTable* table)
			{
				int i = 0;
				
					while (
						lexicMachine->GetActualToken()->GetLex() != ":"||
						lexicMachine->GetActualToken()->GetLex() != ";")
					{
						if (lexicMachine->GetActualToken()->GetType() == "ID")
						{
							AddBlob(lexicMachine->GetActualToken()); ++i;
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
									while (lexicMachine->GetActualToken()->GetLex()!=";")
									{
										lexicMachine->GetNextToken();
									}
								}
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
						//At least we have som var a: int; and therefore we can dump our shit from here
						AddtoTable(table);
						return std::unique_ptr<SintaxState_Start>();
					}
				
				return Process(lexicMachine, errorModule, table);
			}
		};
		class SintaxState_Function : public SintaxState
		{
			std::unique_ptr<SintaxState>  Process(
				Compiler_Lexicon* lexicMachine,
				Compiler_ErrorModule^ errorModule,
				Compiler_SymbolsTable* table)
			{
				//return Process(lexicMachine, errorModule, table);
				if (lexicMachine->GetActualToken()->GetType()=="ID")
				{
					AddBlob(lexicMachine->GetActualToken());
					if (lexicMachine->PeekNextToken()->GetLex() == "(")
					{
						lexicMachine->GetNextToken();
						return std::unique_ptr<SintaxState>();
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
		static class SintaxState_Parameter : public SintaxState
		{
			std::unique_ptr<SintaxState>  Process(
				Compiler_Lexicon* lexicMachine,
				Compiler_ErrorModule^ errorModule,
				Compiler_SymbolsTable* table)
			{
					if (lexicMachine->GetActualToken()->GetType() == "ID")
					{
						AddBlob(lexicMachine->GetActualToken());
					}
					else if (lexicMachine->GetActualToken()->GetLex() == ")")
					{
						//State is finished, therefore we expect a block or a ;
						if (lexicMachine->PeekNextToken()->GetLex() == "{")
						{
							//Return BLOCK STATE WHICH IS NOT YET IMPLEMENTED

						}
						else if (lexicMachine->PeekNextToken()->GetLex() == ";")
						{
							//That means that we have a function a (...); and we finish
							
						}
						else
						{
							AddtoTable(table);
						}
					}
					else
					{
						// We found some dumb shit inside, dont know what we could find
						AddSintaxError(
							errorModule,
							lexicMachine->GetNextToken(),
							"the fuck u doin m8");
					}
				return Process(lexicMachine,errorModule,table);
			}
		};
		//Member declaration for SintaxStates
	public:
		Compiler_SintaxStates()
		{
			m_state = std::make_unique<SintaxState_Start>();
		}
		~Compiler_SintaxStates()
		{
			m_state.release();
		}
		void Run(
			Compiler_Lexicon* lexicMachine,
			Compiler_ErrorModule^ errorModule,
			Compiler_SymbolsTable* table)
		{
			do
			{
				m_state = m_state->Process(lexicMachine, errorModule, table);
			} while (m_state != nullptr);
		}
		
		std::unique_ptr<SintaxState> m_state;
		
		void Empty()
		{
			m_state.release();
			m_state->m_blobVector.clear();
			m_state->m_sintaxstack.empty();
		}
	};

}


