#include "stdafx.h"
#include "Compilers_Core.h"
using namespace CompilersCore;

Manager::Manager()
{

}
Manager::~Manager()
{

}
cli::array<String^>^ Manager::compile(String^ src)
{
	//src is the whole input of the user, the whole code to compile
	//This is the entry of the DLLs
	cli::array<String^>^ compilationDetails;
	compilationDetails = gcnew cli::array<String^>(1);
	compilationDetails[0] = gcnew String("Hola Mundo");
	//gcnew doesnt needs to be deleted, when the context dies, gcnew variables die with it
	return compilationDetails;
}