#pragma once
#include "Compiler_Token.h"
#include <string.h>
#include <vector>
using namespace System;
namespace CompilerCore
{
	enum class NODE_TYPE
	{
		UNDEFINED = 0,
		GLOBAL_NODE,
		LOCAL_NODE,
		PARAMETER,
		FUNCTION
	};
	enum class VAR_TYPE
	{
		UNDEFINED = 0,
		INT,
		FLOAT,
		CHAR,
	};

	public class Node
	{
	public:
		NODE_TYPE m_NodeType;
		std::string m_varType;
		int m_iArraySize = -1;
		int m_lineNumber = -1;
		std::string m_nodeName;
		Node* pLocalNode;
		std::string m_functionParent;
	public:
		Node()
		{

		}
		~Node()
		{

		}
		std::string GetName()
		{
			return m_nodeName;
		}
		
		int GetDimension()
		{
			return m_iArraySize;
		}
		void SetLocalNode(Node& nextNode)
		{
			*pLocalNode = nextNode;
		}
		virtual void 
			SetNode(
				std::string symbol,
				NODE_TYPE nodeType,
				int dimension,
				std::string varType,
				int lineNum,
				std::string
				functionName) = 0;
	};
	public class Compiler_LocalNode : public Node
	{
	public:
		Compiler_LocalNode()
		{

		}
		~Compiler_LocalNode()
		{

		}
		void 
			SetNode(
			std::string symbol, 
			NODE_TYPE nodeType, 
			int dimension, 
			std::string varType, 
			int lineNum, 
			std::string functionName)
		{
			m_nodeName = symbol;
			m_NodeType = nodeType;
			m_iArraySize = dimension;
			m_varType = varType;
			m_lineNumber = lineNum;
			m_functionParent = functionName;
		}
	};
	public class Compiler_GlobalNode : public Node
	{
	public:
		Compiler_GlobalNode()
		{

		}
		~Compiler_GlobalNode()
		{

		}
		void 
			SetNode(
			std::string symbol,
			NODE_TYPE nodeType,
			int dimension,
			std::string varType,
			int lineNum,
			std::string functionName)
		{
			m_nodeName = symbol;
			m_NodeType = nodeType;
			m_iArraySize = dimension;
			m_varType = varType;
			m_lineNumber = lineNum;
			m_functionParent = functionName;
		}
	};
}


