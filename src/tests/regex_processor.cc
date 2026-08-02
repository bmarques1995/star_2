#include <gtest/gtest.h>
#include <string>
#include "RegexProcessor.hh"

namespace star
{
    TEST(regex_processor, failed_string)
    {
        std::string input = "7950u32";
        std::string pattern = R"(\A(?<number>([0-9](?:[0-9]|(?:'[0-9]{3}))*))(?<type>i(?:8|16|32|64)|u(?:8|16|32|64))?(?=[\+\-\*\/\%; \t\r\n]|\z))";
        RegexProcessor processor(pattern);
        auto matches = processor.GetMatches(input);
        EXPECT_EQ(matches.size(),4);
    }
}
