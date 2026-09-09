#pragma once

#include <memory>
#include <vector>
#include "Value.hh"
#include "StarMacro.hh"

namespace star
{
	class Interpreter;

    class STAR_API Callable
    {
    public:
        virtual ~Callable() = default;

		virtual const size_t Arity() const = 0;
		virtual std::string ToString() const = 0;

        virtual Value Call(Interpreter& interpreter, std::vector<Value> args) = 0;
    };
}