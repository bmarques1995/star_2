#ifdef STAR_DEBUG

#include <gtest/gtest.h>
#include <string>
#include "Token.hh"

namespace star
{
    TEST(token, str_type)
    {
        Token token(TokenType::STRING, "value", "\"value\"", 1);
        std::string expectedOutput = "";
        std::string actualOutput = token.ToString();
        EXPECT_NE(actualOutput, expectedOutput);
    }
    TEST(token, var_type)
    {
        Token token(TokenType::VAR, "x", nullptr, 1);
        std::string expectedOutput = "";
        std::string actualOutput = token.ToString();
        EXPECT_NE(actualOutput, expectedOutput);
    }
    
}

#endif