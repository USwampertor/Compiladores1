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
		NODE_TYPE nodeType;
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
			SintaxState()
			{

			}
			~SintaxState()
			{

			}
			virtual std::unique_ptr<SintaxState>
				Process() = 0;
			void AddError()
			{
			}
		};
		class SintaxState_Start : public SintaxState
		{
			std::unique_ptr<SintaxState>  Process()
			{
				return Process();
			}
		};
		class SintaxState_Var : public SintaxState
		{

			std::unique_ptr<SintaxState>  Process()
			{

				return Process();
			}
		};
		class SintaxState_Function : public SintaxState
		{
			std::unique_ptr<SintaxState>  Process()
			{
				return Process();
			}
		};
		class SintaxState_Parameter : public SintaxState
		{
			std::unique_ptr<SintaxState>  Process()
			{
				return Process();
			}
		};
		class SintaxState_Error : public SintaxState
		{
			std::unique_ptr<SintaxState>  Process()
			{
				return Process();
			}
		};
		//Member declaration for SintaxStates
		public:
		Compiler_SintaxStates(
			Compiler_Lexicon* lexicMachine,
			Compiler_ErrorModule^ errorModule,
			Compiler_SymbolsTable* table)
		{
			m_state = std::make_unique<SintaxState_Start>();
		}
		~Compiler_SintaxStates()
		{
			m_state.release();
		}
		void Run()
		{
			do
			{
				m_state->Process();
			} while (m_state != nullptr);
		}
		std::stack<std::unique_ptr<SintaxState>> m_sintaxstack;
		static std::unique_ptr<SintaxState> m_state;
		std::vector<SintaxBlob> m_blobVector;
		void Empty()
		{
			m_sintaxstack.empty();
			m_state.release();
			m_blobVector.clear();
		}
	};

}


