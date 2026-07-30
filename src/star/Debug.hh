#pragma once

#include "StarMacro.hh"
#include <cstdint>
#include <string>

namespace star
{
    class STAR_API Debug
    {
    private:
        static void Report(uint32_t line, const std::string& where, const std::string& message);

        inline static bool s_HadError = false;

    public:
        static void Error(uint32_t line, const std::string&);
        static bool HadError();
    };
}