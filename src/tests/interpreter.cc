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
        std::string result = interpreter.Interpret(expr).ToString();
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
        std::string result = interpreter.Interpret(expr).ToString();
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
        std::string result = interpreter.Interpret(expr).ToString();
        EXPECT_EQ(result, "7");
    }

    TEST(interpreter, equal_equal_ternary)
    {
        std::string input = "4 == 5 ? 12 : 7";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        Parser parser{tokens};
        auto expr = parser.Parse();
        Interpreter interpreter{};
        std::string result = interpreter.Interpret(expr).ToString();
        EXPECT_EQ(result, "7");
    }

    TEST(interpreter, bang_equal_ternary)
    {
        std::string input = "4 != 5 ? 12 : 7";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        Parser parser{tokens};
        auto expr = parser.Parse();
        Interpreter interpreter{};
        std::string result = interpreter.Interpret(expr).ToString();
        EXPECT_EQ(result, "12");
    }

    TEST(interpreter, linear_expression)
    {
        std::string input = "6 * 2 + 3";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        Parser parser{tokens};
        auto expr = parser.Parse();
        Interpreter interpreter{};
        std::string result = interpreter.Interpret(expr).ToString();
        EXPECT_EQ(result, "15");
    }

    TEST(interpreter, grouped_expression)
    {
        std::string input = "6 * (2 + 3)";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        Parser parser{tokens};
        auto expr = parser.Parse();
        Interpreter interpreter{};
        std::string result = interpreter.Interpret(expr).ToString();
        EXPECT_EQ(result, "30");
    }

    TEST(interpreter, negate_true_expression)
    {
        std::string input = "!true";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        Parser parser{tokens};
        auto expr = parser.Parse();
        Interpreter interpreter{};
        std::string result = interpreter.Interpret(expr).ToString();
        EXPECT_EQ(result, "false");
    }

    TEST(interpreter, negate_false_expression)
    {
        std::string input = "!false";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        Parser parser{tokens};
        auto expr = parser.Parse();
        Interpreter interpreter{};
        std::string result = interpreter.Interpret(expr).ToString();
        EXPECT_EQ(result, "true");
    }

    TEST(interpreter, negate_number)
    {
        std::string input = "-5";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        Parser parser{tokens};
        auto expr = parser.Parse();
        Interpreter interpreter{};
        std::string result = interpreter.Interpret(expr).ToString();
        EXPECT_EQ(result, "-5");
    }

    TEST(interpreter, template_string)
    {
        std::string input = R"(`This is a template string, to prove, use these expressions: ${1995/35}$ and ${143-13}$`)";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        Parser parser{tokens};
        auto expr = parser.Parse();
        Interpreter interpreter{};
        std::string result = interpreter.Interpret(expr).ToString();
        EXPECT_EQ(result, "This is a template string, to prove, use these expressions: 57 and 130");
    }

    TEST(interpreter, greater_tests)
    {
        std::string input = R"(4 > 5 ? 4 > 5 : 5 > 4)";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        Parser parser{tokens};
        auto expr = parser.Parse();
        Interpreter interpreter{};
        std::string result = interpreter.Interpret(expr).ToString();
        EXPECT_EQ(result, "true");
    }

    TEST(interpreter, greater_equal_tests)
    {
        std::string input = R"(4 >= 5 ? 4 >= 5 : 5 >= 4)";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        Parser parser{tokens};
        auto expr = parser.Parse();
        Interpreter interpreter{};
        std::string result = interpreter.Interpret(expr).ToString();
        EXPECT_EQ(result, "true");
    }

    TEST(interpreter, less_tests)
    {
        std::string input = R"(4 < 5 ? 5 < 4 : 4 < 5)";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        Parser parser{tokens};
        auto expr = parser.Parse();
        Interpreter interpreter{};
        std::string result = interpreter.Interpret(expr).ToString();
        EXPECT_EQ(result, "false");
    }

    TEST(interpreter, less_equal_tests)
    {
        std::string input = R"(4 <= 5 ? 5 <= 4 : 4 <= 5)";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        Parser parser{tokens};
        auto expr = parser.Parse();
        Interpreter interpreter{};
        std::string result = interpreter.Interpret(expr).ToString();
        EXPECT_EQ(result, "false");
    }

    TEST(interpreter, mod_tests)
    {
        std::string input = R"(print(3%2))";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        Parser parser{tokens};
        auto expr = parser.Parse();
        Interpreter interpreter{};
        std::string result = interpreter.Interpret(expr).ToString();
        EXPECT_EQ(result, "1");
    }
}