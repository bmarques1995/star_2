#pragma once

#include "StarDevMacro.hh"
#include <string>

namespace star
{
    class STAR_DEV_API Debug
    {
    private:
        static void Report(size_t line, const std::string& where, const std::string& message);

        inline static bool s_HadError = false;
        inline static std::string s_Report;

    public:
        static const std::string& GetLastReport();
        static void Error(size_t line, const std::string&);
        static bool HadError();
    };
}