#pragma once

#include <memory>
#include <vector>
#include "Callable.hh"
#include "StarMacro.hh"
#include "Value.hh"
#include "Stmt.hh"
#include "Environment.hh"

namespace star
{
    class STAR_API Function : public Callable
    {
    public:
        Function(std::shared_ptr<Statement::Function> declaration, std::shared_ptr<Environment> chaining);

        const size_t Arity() const override;
        std::string ToString() const override;
        Value Call(Interpreter& interpreter, std::vector<Value> args) override;

    private:
        std::shared_ptr<Statement::Function> m_Declaration;
		std::shared_ptr<Environment> m_Chaining;
    };
}