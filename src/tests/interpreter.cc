#include <gtest/gtest.h>
#include <string>
#include "Scanner.hh"
#include "Parser.hh"
#include "Interpreter.hh"

namespace star
{

    class MockInterpreter : public Interpreter
    {
    public:
        MockInterpreter()
        {

        }
        ~MockInterpreter() = default;

        void Write(const std::string& text) const override
        {
            Interpreter::Write(text);
            m_Result = text;
        }

        const std::string& GetText()
        {
            return m_Result;
        }

    private:
        mutable std::string m_Result;
    };

    TEST(interpreter, expression)
    {
        std::string input = "print(\"Hello String\");";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        Parser parser{tokens};
        auto expr = parser.Parse();
        MockInterpreter interpreter{};
        interpreter.Interpret(expr);
        EXPECT_EQ(interpreter.GetText(), "Hello String");
    }

    TEST(interpreter, ternary_true_expression)
    {
        std::string input = "print(4 < 5 ? 12 : 7);";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        Parser parser{tokens};
        auto expr = parser.Parse();
        MockInterpreter interpreter{};
        interpreter.Interpret(expr);
        EXPECT_EQ(interpreter.GetText(), "12");
    }

    TEST(interpreter, ternary_false_expression)
    {
        std::string input = "print(4 > 5 ? 12 : 7);";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        Parser parser{tokens};
        auto expr = parser.Parse();
        MockInterpreter interpreter{};
        interpreter.Interpret(expr);
        EXPECT_EQ(interpreter.GetText(), "7");
    }

    TEST(interpreter, equal_equal_ternary)
    {
        std::string input = "print(4 == 5 ? 12 : 7);";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        Parser parser{tokens};
        auto expr = parser.Parse();
        MockInterpreter interpreter{};
        interpreter.Interpret(expr);
        EXPECT_EQ(interpreter.GetText(), "7");
    }

    TEST(interpreter, bang_equal_ternary)
    {
        std::string input = "print(4 != 5 ? 12 : 7);";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        Parser parser{tokens};
        auto expr = parser.Parse();
        MockInterpreter interpreter{};
        interpreter.Interpret(expr);
        EXPECT_EQ(interpreter.GetText(), "12");
    }

    TEST(interpreter, linear_expression)
    {
        std::string input = "print(6 * 2 + 3);";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        Parser parser{tokens};
        auto expr = parser.Parse();
        MockInterpreter interpreter{};
        interpreter.Interpret(expr);
        EXPECT_EQ(interpreter.GetText(), "15");
    }

    TEST(interpreter, grouped_expression)
    {
        std::string input = "print(6 * (2 + 3));";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        Parser parser{tokens};
        auto expr = parser.Parse();
        MockInterpreter interpreter{};
        interpreter.Interpret(expr);
        EXPECT_EQ(interpreter.GetText(), "30");
    }

    TEST(interpreter, negate_true_expression)
    {
        std::string input = "print(!true);";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        Parser parser{tokens};
        auto expr = parser.Parse();
        MockInterpreter interpreter{};
        interpreter.Interpret(expr);
        EXPECT_EQ(interpreter.GetText(), "false");
    }

    TEST(interpreter, negate_false_expression)
    {
        std::string input = "print(!false);";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        Parser parser{tokens};
        auto expr = parser.Parse();
        MockInterpreter interpreter{};
        interpreter.Interpret(expr);
        EXPECT_EQ(interpreter.GetText(), "true");
    }

    TEST(interpreter, negate_number)
    {
        std::string input = "print(-5);";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        Parser parser{tokens};
        auto expr = parser.Parse();
        MockInterpreter interpreter{};
        interpreter.Interpret(expr);
        EXPECT_EQ(interpreter.GetText(), "-5");
    }

    TEST(interpreter, template_string)
    {
        std::string input = R"(print(`This is a template string, to prove, use these expressions: ${1995/35}$ and ${143-13}$`);)";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        Parser parser{tokens};
        auto expr = parser.Parse();
        MockInterpreter interpreter{};
        interpreter.Interpret(expr);
        EXPECT_EQ(interpreter.GetText(), "This is a template string, to prove, use these expressions: 57 and 130");
    }

    TEST(interpreter, greater_tests)
    {
        std::string input = R"(print(4 > 5 ? 4 > 5 : 5 > 4);)";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        Parser parser{tokens};
        auto expr = parser.Parse();
        MockInterpreter interpreter{};
        interpreter.Interpret(expr);
        EXPECT_EQ(interpreter.GetText(), "true");
    }

    TEST(interpreter, greater_equal_tests)
    {
        std::string input = R"(print(4 >= 5 ? 4 >= 5 : 5 >= 4);)";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        Parser parser{tokens};
        auto expr = parser.Parse();
        MockInterpreter interpreter{};
        interpreter.Interpret(expr);
        EXPECT_EQ(interpreter.GetText(), "true");
    }

    TEST(interpreter, less_tests)
    {
        std::string input = R"(print(4 < 5 ? 5 < 4 : 4 < 5);)";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        Parser parser{tokens};
        auto expr = parser.Parse();
        MockInterpreter interpreter{};
        interpreter.Interpret(expr);
        EXPECT_EQ(interpreter.GetText(), "false");
    }

    TEST(interpreter, less_equal_tests)
    {
        std::string input = R"(print(4 <= 5 ? 5 <= 4 : 4 <= 5);)";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        Parser parser{tokens};
        auto expr = parser.Parse();
        MockInterpreter interpreter{};
        interpreter.Interpret(expr);
        EXPECT_EQ(interpreter.GetText(), "false");
    }

    TEST(interpreter, mod_tests)
    {
        std::string input = R"(print(3%2);)";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        Parser parser{tokens};
        auto expr = parser.Parse();
        MockInterpreter interpreter{};
        interpreter.Interpret(expr).ToString();
        EXPECT_EQ(interpreter.GetText(), "1");
    }
}