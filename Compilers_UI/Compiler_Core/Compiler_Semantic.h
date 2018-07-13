#pragma once
#include "Compiler_Token.h"
#include "Compiler_ErrorModule.h"
#include "Compiler_SymbolsTable.h"
#include <string.h>
#include <vector>
#include <map>
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
		void AddLogicalExp(std::vector<Compiler_Token*>);
		bool ValidateExpression();
	private:
		std::multimap<std::string, std::vector<Compiler_Token*>> m_expMap;
		void InFixToPostFix(std::vector<Compiler_Token*>);
		std::string CreateStringKey(std::string funcname, int linenum);
	};

}

