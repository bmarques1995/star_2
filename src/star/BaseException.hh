#pragma once

#include "StarMacro.hh"
#include <string>

namespace star 
{
    class STAR_API ScriptException
    {
    public:
        ScriptException();
        virtual ~ScriptException() = default;
    protected:
        std::string m_Reason;
    };
}
