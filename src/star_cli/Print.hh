#pragma once

#include "StarCLIMacro.hh"
#include "Callable.hh"

namespace star
{
	//fun print(value)#void;
	class STAR_CLI_API Print : public Callable
	{
	public:
		Print();
		~Print() = default;
		
		const size_t Arity() const override;
		std::string ToString() const override;

		Value Call(Interpreter& interpreter, std::vector<Value> args) override;
	public:
		size_t m_Arity;

	};
}