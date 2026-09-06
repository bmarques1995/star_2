#pragma once

#include "StarMacro.hh"
#include <string>

namespace star
{
	class STAR_API Version
	{
	public:
		static std::string GetVersion();
		static std::string GetCompilerNameAndVersion();
	};
}
