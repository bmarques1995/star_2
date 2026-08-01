#pragma once

#include "StarCLIMacro.hh"
#include <string>

namespace star
{
    class STAR_CLI_API TraceConsole
    {
    public:
        TraceConsole& operator<<(const std::string& value);    
    };

    class STAR_CLI_API WarnConsole
    {
    public:
        WarnConsole& operator<<(const std::string& value);
    };

    class STAR_CLI_API ErrorConsole
    {
    public: 
        ErrorConsole& operator<<(const std::string& value);
    };
}