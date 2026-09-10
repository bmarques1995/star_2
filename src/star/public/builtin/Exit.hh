#pragma once

#include "StarMacro.hh"
#include "Callable.hh"

namespace star
{
	// fun exit(code#u32)#void
	class STAR_API Exit : public Callable
	{
	public:
		Exit();
		~Exit() = default;

		const size_t Arity() const override;
		std::string ToString() const override;
		
		Value Call(Interpreter& interpreter, std::vector<Value> args) override;
	private:
		size_t m_Arity;
		std::vector<VariableType> m_ExpectedTypes;
	};
}