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
				m_blobVector.push_back(blob);
			}
			void AddtoTable(Compiler_SymbolsTable* st, NODE_TYPE nodeType)
			{
				for (int i = 0; i< m_blobVector.size(); ++i)
				{
					st->AddSymbol(
						m_blobVector[i].symbol, 
						nodeType,
						m_blobVector[i].dimension, 
						m_blobVector[i].varType, 
						m_blobVector[i].lineNum,
						m_blobVector[i].functionName);
				}
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
					return std::unique_ptr<SintaxState_Var>();
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
				if (lexicMachine->PeekNextToken()->GetType()=="ID")
				{
					AddBlob(lexicMachine->GetNextToken());
					if (lexicMachine->PeekNextToken()->GetType() == "RELATIONAL OPERATOR" &&
						lexicMachine->PeekNextToken()->GetLex() == "[")
					{
						lexicMachine->GetNextToken();
						if (lexicMachine->PeekNextToken()->GetType() == "INT")
						{
							m_blobVector.back().dimension = atoi(lexicMachine->PeekNextToken()->GetLex().c_str());
						}
						else
						{

						}
					}
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
				return Process(lexicMachine, errorModule, table);
			}
		};
		static class SintaxState_Parameter : public SintaxState
		{
			std::unique_ptr<SintaxState>  Process(
				Compiler_Lexicon* lexicMachine,
				Compiler_ErrorModule^ errorModule,
				Compiler_SymbolsTable* table)
			{
				return Process(lexicMachine,errorModule,table);
			}
		};
		//Member declaration for SintaxStates
	private:
		Compiler_Lexicon * m_lexicMachine;
		Compiler_SymbolsTable* m_symbolsTable;
		msclr::gcroot<Compiler_ErrorModule^> m_error;
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
		
		static std::unique_ptr<SintaxState> m_state;
		
		void Empty()
		{
			m_state.release();
			m_state->m_blobVector.clear();
			m_state->m_sintaxstack.empty();
		}
	};

}


