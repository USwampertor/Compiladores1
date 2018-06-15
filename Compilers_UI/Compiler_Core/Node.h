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
		INT = 0,
		FLOAT,
		CHAR,
	};

	public class Node
	{
	public:
		NODE_TYPE m_NodeType;
		VAR_TYPE m_varType;
		int m_iArraySize = -1;
		std::string m_nodeName;
		Node* pLocalNode;
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
		virtual void SetNode() = 0;
	};
	public class Compiler_LocalNode : public Node
	{
	private:
		std::string context;
	public:
		Compiler_LocalNode()
		{

		}
		~Compiler_LocalNode()
		{

		}
		void SetNode()
		{
			
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
		void SetNode()
		{

		}
	};
}


