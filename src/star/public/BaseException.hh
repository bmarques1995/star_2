#pragma once

#include "StarMacro.hh"
#include <exception>
#include <string>

namespace star 
{
    class STAR_API ScriptException : public std::exception
    {
    public:
        ScriptException();
        virtual ~ScriptException() = default;

        const char* what() const noexcept override;
    protected:
        std::string m_Reason;
    };
}
