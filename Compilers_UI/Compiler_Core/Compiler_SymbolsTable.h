#pragma once
#include <map>
#include "Node.h"
#include <string.h>
using namespace System;
namespace CompilerCore
{
	
	public class Compiler_SymbolsTable
	{
	private:
		std::map<std::string, int> m_VARTABLE;
		std::map<std::string, Node*> m_HashTable;
		std::map<int, std::string> m_NODETABLE;
	public:
		Compiler_SymbolsTable();
		~Compiler_SymbolsTable();

		bool AddSymbol(
			std::string symbol, 
			NODE_TYPE nodeType, 
			int dimension, 
			std::string varType, 
			int lineNum,
			std::string functionName);

		bool SymbolExists(std::string symbol);
		bool SymbolExists(std::string symbol, NODE_TYPE type);
		std::string GetSymbolType(std::string symbol, NODE_TYPE type);
		void DeleteNode(std::string symbol, NODE_TYPE type, std::string functionName);
		void Clear();
		Node* FinalNode(Node* actualNode);
		bool NextNode(Node* actualNode, NODE_TYPE type);
		std::string NextString(Node* actualNode, NODE_TYPE type);
	};
}

