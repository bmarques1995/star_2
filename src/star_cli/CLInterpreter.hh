#pragma once

#include "Interpreter.hh"
#include "StarCLIMacro.hh"

namespace star
{
    class STAR_CLI_API CLInterpreter : public Interpreter
    {
    public:
        CLInterpreter();
        ~CLInterpreter() = default;
    };
}
