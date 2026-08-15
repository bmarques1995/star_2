#pragma once

#include "BaseException.hh"
#include "StarMacro.hh"
#include "Token.hh"

namespace star
{
    class STAR_API RuntimeError : public ScriptException
    {
    public:
        const Token& token;
        RuntimeError(const Token& token, const std::string& message);
    };
}