#include <gtest/gtest.h>
#include <string>
#include "Scanner.hh"
#include "Parser.hh"
#include "Interpreter.hh"

namespace star
{

    TEST(interpreter, expression)
    {
        std::string input = "\"Hello String\"";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        Parser parser{tokens};
        auto expr = parser.Parse();
        Interpreter interpreter{};
        std::string result = interpreter.Interpret(expr);
        EXPECT_EQ(result, "Hello String");
    }

    TEST(interpreter, ternary_true_expression)
    {
        std::string input = "4 < 5 ? 12 : 7";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        Parser parser{tokens};
        auto expr = parser.Parse();
        Interpreter interpreter{};
        std::string result = interpreter.Interpret(expr);
        EXPECT_EQ(result, "12");
    }

    TEST(interpreter, ternary_false_expression)
    {
        std::string input = "4 > 5 ? 12 : 7";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        Parser parser{tokens};
        auto expr = parser.Parse();
        Interpreter interpreter{};
        std::string result = interpreter.Interpret(expr);
        EXPECT_EQ(result, "7");
    }
}