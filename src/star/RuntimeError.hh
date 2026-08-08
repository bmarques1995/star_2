#pragma once

#include "StarMacro.hh"
#include "Token.hh"
#include "BaseException.hh"

namespace star
{
    class STAR_API RuntimeError : public ScriptException
    {
        public:
        const Token& m_Token;
        RuntimeError(const Token& token, const std::string& message);
    };
}

