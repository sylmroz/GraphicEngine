#pragma once

#define GCC_COMPILER (defined(__GNUC__) && !defined(__clang__))

#include <string>

#if GCC_COMPILER
#include <cxxabi.h>
#endif

namespace GraphicEngine::Core::Utils
{
	template <typename T>
	std::string getClassName()
	{
#if GCC_COMPILER
		int status;
		return std::string(abi::__cxa_demangle(typeid(A).name(), 0, 0, &status));
#else
		return typeid(T).name();
#endif
	}
}