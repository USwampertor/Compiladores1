#pragma once

using namespace System;

namespace CompilersCore {
	public ref class Manager
	{
	public:
		Manager();
		~Manager();
		cli::array<String^>^  compile(String^ src);

		// TODO: Agregar aquí los métodos de la clase.
	};
}
