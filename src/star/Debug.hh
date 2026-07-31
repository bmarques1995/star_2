#pragma once

#include "StarMacro.hh"
#include <string>

namespace star
{
    class STAR_API Debug
    {
    private:
        static void Report(size_t line, const std::string& where, const std::string& message);

        inline static bool s_HadError = false;

    public:
        static void Error(size_t line, const std::string&);
        static bool HadError();
    };
}