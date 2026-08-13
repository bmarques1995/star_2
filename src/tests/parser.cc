#include <gtest/gtest.h>
#include <string>
#include "Scanner.hh"
#include "TokenType.hh"
#include "Parser.hh"

namespace star
{
    TEST(parser, keyword)
    {
        std::string input = "if";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        Parser parser{tokens};
        EXPECT_THROW(parser.Parse(), ParserException);
    }

    TEST(parser, unterminated_template_expr)
    {
        std::vector<Token> tokens = 
        {
            {TokenType::TEMPLATE_STRING_START, "", 1, 0, "::repl"},
            {TokenType::TEMPLATE_SUBSTRING, "Jumper ", 1, 1, "::repl"},
            {TokenType::STR_EXPR_START, "", 1, 7, "::repl"},
            {TokenType::NUMBER, "78", 1, 10, "::repl"},
            {TokenType::TEMPLATE_STRING_END, "", 1, 1, "::repl"},
        };
        Parser parser{tokens};
        EXPECT_THROW(parser.Parse(), ParserException);
    }

    TEST(parser, unterminated_template_str)
    {
        std::vector<Token> tokens = 
        {
            {TokenType::TEMPLATE_STRING_START, "", 1, 0, "::repl"},
            {TokenType::TEMPLATE_SUBSTRING, "Jumper ", 1, 1, "::repl"},
            {TokenType::STR_EXPR_START, "", 1, 7, "::repl"},
            {TokenType::NUMBER, "78", 1, 10, "::repl"},
            {TokenType::STR_EXPR_END, "", 1, 12, "::repl"},
            {TokenType::ST_EOF, "", 2, 1, "::repl"}
        };
        Parser parser{tokens};
        EXPECT_THROW(parser.Parse(), ParserException);
    }
}