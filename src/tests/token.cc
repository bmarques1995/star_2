#ifdef STAR_DEBUG

#include <gtest/gtest.h>
#include <string>
#include "Token.hh"

namespace star
{
    TEST(token, str_type)
    {
        Token token(TokenType::STRING, "value", 1, 1, "::repl");
        std::string expectedOutput = "";
        std::string actualOutput = token.ToString();
        EXPECT_NE(actualOutput, expectedOutput);
    }
    TEST(token, var_type)
    {
        Token token(TokenType::VAR, "x", 1, 1, "::repl");
        std::string expectedOutput = "";
        std::string actualOutput = token.ToString();
        EXPECT_NE(actualOutput, expectedOutput);
    }

    TEST(token, int_type)
    {
        Token token(TokenType::NUMBER, "10", 1, 1, "::repl");
        std::string expectedOutput = "NUMBER 10 at: ::repl:1:1";
        std::string actualOutput = token.ToString();
        EXPECT_EQ(actualOutput, expectedOutput);
    }

    TEST(token, out_operator)
    {
        Token token(TokenType::PLUS, "+", 1, 1, "::repl");
        std::stringstream actualOutput;
        actualOutput << token;
        std::string expectedOutput = "PLUS + at: ::repl:1:1";
        EXPECT_EQ(actualOutput.str(), expectedOutput);
    }
    
    TEST(token, no_literal)
    {
        Token token(TokenType::IDENTIFIER, "D3DName", 1, 1, "::repl");
        std::stringstream actualOutput;
        actualOutput << token;
        std::string expectedOutput = "IDENTIFIER D3DName at: ::repl:1:1";
        EXPECT_EQ(actualOutput.str(), expectedOutput);
    }
}

#endif