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
		std::shared_ptr<Environment> m_Parent;

	public:
		Environment();
		Environment(std::shared_ptr<Environment> parent);

		void Define(const Token& name, Value value);
		void Reassign(const Token& name, Value value);
		Value Get(const Token& name);
	};
}