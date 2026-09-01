#pragma once

#include <unordered_map>
#include "Value.hh"
#include <memory>
#include "Token.hh"
#include "StarMacro.hh"

namespace star
{
	class STAR_API Environment : public std::enable_shared_from_this<Environment>
	{
	private:
		std::unordered_map<std::string, Value> m_Values;

	public:
		Environment();

		void Define(const Token& name, Value value);
		void Reassign(const Token& name, Value value);
		Value Get(const Token& name);
	};
}