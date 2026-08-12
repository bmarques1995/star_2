#pragma once

#include "BaseException.hh"
#include "Token.hh"

namespace star
{
    class RuntimeError : public ScriptException
    {
    public:
        const Token& token;
        RuntimeError(const Token& token, const std::string& message);
    };
}