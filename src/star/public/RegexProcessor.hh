#pragma once

#include "StarMacro.hh"
#include <cstddef>
#include <utility>
#include <vector>
#include <string>
#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>

namespace star
{
    class STAR_API RegexProcessor
    {
    public:
        RegexProcessor(std::string_view pattern);
        ~RegexProcessor();

        std::vector<std::pair<size_t, size_t>> GetMatches(std::string_view text);
        std::pair<size_t, size_t> GetFirstMatch(std::string_view text);
    private:
        std::string m_Pattern;
        pcre2_code *m_RE;

        void GetLastError(int code);
        int AnalyzeExpression(std::string_view text, pcre2_match_data** match_data, size_t** ovector);
    };
}
