#include "stdafx.h"
#include "Compiler_SymbolsTable.h"

using namespace CompilerCore;
Compiler_SymbolsTable::Compiler_SymbolsTable()
{
	m_HashTable.clear();
	m_NODETABLE.insert(std::make_pair(0, "UNDEFINED"));
	m_NODETABLE.insert(std::make_pair(1, "GLOBAL_NODE"));
	m_NODETABLE.insert(std::make_pair(2, "LOCAL_NODE"));
	m_NODETABLE.insert(std::make_pair(3, "PARAMETER"));
	m_NODETABLE.insert(std::make_pair(4, "FUNCTION"));
	m_VARTABLE.insert(std::make_pair("INT",0));
	m_VARTABLE.insert(std::make_pair("FLOAT",1));
	m_VARTABLE.insert(std::make_pair("CHAR",2));
}


Compiler_SymbolsTable::~Compiler_SymbolsTable()
{
}
bool Compiler_SymbolsTable::AddSymbol(
	std::string symbol, 
	NODE_TYPE nodeType, 
	int dimension, 
	std::string varType, 
	int lineNum,
	std::string functionName)
{
	if (m_HashTable.find(symbol) != m_HashTable.end())
	{
		//We do have either a GLOBAL or LOCAL nodes with given symbol name
		//Now we have to check how many GLOBAL or LOCAL nodes we have with that name
		if (SymbolExists(symbol,nodeType))
		{
			return false;
		}
		auto it = m_HashTable.find(symbol);
		Compiler_LocalNode* pLocal = new Compiler_LocalNode();
		//pLocal->SetNode();
		FinalNode(it->second)->pLocalNode = pLocal;
		return true;
	}
	else if (m_HashTable.find(symbol) == m_HashTable.end())
	{
		//We don't have any GLOBAL or LOCAL nodes with given symbol name
		//We have to see if this is a local or a global node
		
		if (nodeType != NODE_TYPE::GLOBAL_NODE)
		{
			//This means we have a local variable without a global before
			//Therefore we need to create a global undefined Variable
			Compiler_GlobalNode* pGlobal = new Compiler_GlobalNode();
			//globalVariable->SetNode(); WITH UNDEFINED AS VARTYPE
			m_HashTable.insert(std::make_pair(symbol, pGlobal));
			Compiler_LocalNode* pLocal = new Compiler_LocalNode();
			//pLocal->SetNode();
			FinalNode(pGlobal)->pLocalNode = pLocal;
		}
		else
		{
			Compiler_LocalNode* pLocal = new Compiler_LocalNode();
			//pLocal->SetNode();
			m_HashTable.insert(std::make_pair(symbol, pLocal));
		}
		return true;

	}
	return false;
}
Node* Compiler_SymbolsTable::FinalNode(Node* actualNode)
{
	//We search for a specific existing node, and then look if his localNode pointer is null
	//if null, we add the new node there,
	if (actualNode->pLocalNode != nullptr)
	{
		return FinalNode(actualNode->pLocalNode);
	}
	//else, we send this function again
	else return actualNode;
}
bool Compiler_SymbolsTable::SymbolExists(std::string symbol)
{
	if (m_HashTable.find(symbol)!=m_HashTable.end())
	{
		return true;
	}
	return false;
}
bool Compiler_SymbolsTable::SymbolExists(std::string symbol, NODE_TYPE type)
{
	if (m_HashTable.find(symbol) != m_HashTable.end())
	{
		auto it = m_HashTable.find(symbol);
		return NextNode(it->second, type);
	}
	return false;
}
bool Compiler_SymbolsTable::NextNode(Node* actualNode, NODE_TYPE type)
{
	if (actualNode->m_NodeType == type)
	{
		return true;
	}
	else if (actualNode->pLocalNode == nullptr)
	{
		return false;
	}
	return NextNode(actualNode->pLocalNode, type);
}
std::string Compiler_SymbolsTable::GetSymbolType(std::string symbol, NODE_TYPE type)
{
	if (m_HashTable.find(symbol) != m_HashTable.end())
	{
		auto it = m_HashTable.find(symbol);
		return NextString(it->second, type);
	}
	return "NONEXISTINGNODE";
}
std::string Compiler_SymbolsTable::NextString(Node* actualNode, NODE_TYPE type)
{
	if (actualNode->m_NodeType == type)
	{
		return m_NODETABLE.find((int)actualNode->m_NodeType)->second;
	}
	else if (actualNode->pLocalNode == nullptr)
	{
		return "NONEXISTINGNODE";
	}
	return NextString(actualNode->pLocalNode, type);
}
void Compiler_SymbolsTable::DeleteNode(std::string symbol, NODE_TYPE type, std::string functionName)
{

}
void Compiler_SymbolsTable::Clear()
{
	m_HashTable.clear();
}