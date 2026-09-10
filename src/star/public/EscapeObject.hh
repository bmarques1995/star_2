#pragma once

#include "StarMacro.hh"

namespace star
{
	// Represents an object that can be escaped from, such as a loop or a function.
	struct STAR_API EscapeObject
	{
		EscapeObject(int escapeCode);

		const int m_EscapeCode;
	};
}