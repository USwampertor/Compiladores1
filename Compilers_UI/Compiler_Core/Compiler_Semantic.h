#pragma once
#include "Compiler_Token.h"
#include "Compiler_ErrorModule.h"
#include "Compiler_SymbolsTable.h"
#include "Compiler_LogExp.h"
#include <string.h>
#include <vector>
#include <map>
using namespace System;
namespace CompilerCore
{
	/*Crear clase semántica e inicializarla en el mamanger
	Pasarlo como parametro al amalizador sintáctico*/
	class Compiler_Semantic
	{
	public:
		Compiler_Semantic(Compiler_ErrorModule^ cterror, Compiler_SymbolsTable* cTable);
		Compiler_Semantic();
		~Compiler_Semantic();
		void AddLogicalExp(
			std::vector<const Compiler_Token*> exp,
			int lineNum,
			std::string funcName,
			std::string statement,
			NODE_RESULT expectedRes,
			std::string symbol,
			int symbolIndex);
		bool ValidateExpression();
		void CreateExpressionTrees();
		std::vector<Compiler_LogExp*> m_notProcessedExpressions;
		void AddSemanticError(Compiler_ErrorModule^ errorm, const Compiler_Token* cToken, char* desc);
	private:
		Compiler_SymbolsTable* m_symbolsTable;
		//msclr::gcroot<Compiler_ErrorModule^> 
		//std::multimap<std::string, std::vector<Compiler_Token*>> m_expMap;
		std::vector<Compiler_LogExp*> m_expVector;
		void InFixToPostFix(std::vector<const Compiler_Token*> inf, std::vector<const Compiler_Token*> postf);
		std::string CreateStringKey(std::string funcname, int linenum);
		
	};

}

