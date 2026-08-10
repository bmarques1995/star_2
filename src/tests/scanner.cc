#include <gtest/gtest.h>
#include <string>
#include "Scanner.hh"

namespace star
{

    TEST(scanner, failed_string)
    {
        std::string input = "var lang = \"something";
        Scanner scanner(input);
        EXPECT_THROW(scanner.ScanTokens(), star::ScannerException);
    }

    TEST(scanner, correct_string)
    {
        std::string input = "var lang = \"something\";";
        Scanner scanner(input);
        EXPECT_NO_THROW(scanner.ScanTokens());
    }

    TEST(scanner, non_closed_multiline_comment)
    {
        std::string input = R"(/* This is a non-closed multiline comment
you can even write code here and it will be ignored
like this: 
var x = 10;)";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        EXPECT_TRUE(tokens.size() == 1);
    }

    TEST(scanner, closed_multiline_comment)
    {
        std::string input = R"(/* This is a closed multiline comment */
var x = 10;)";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        EXPECT_TRUE(tokens.size() > 1);
    }

    TEST(scanner, variable_naming_matching_test)
    {
        std::string input = R"(/* These are some variables */
var x = 10.5;
var y = 20;
var y3 = 20.3e-8f32;
var y4 = .3e-8f32;
var _y = 20u64;
var z = 30;)";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        EXPECT_TRUE(tokens.size() == 31);
    }

    TEST(scanner, variable_naming_non_matching_test)
    {
        std::string input = R"(var 1x = "teste";)";
        Scanner scanner(input);
        EXPECT_THROW(scanner.ScanTokens(), star::ScannerException);
    }

    TEST(scanner, hex_attribute_test)
    {
        std::string input = R"(0x1A3F 0X64ae)";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        EXPECT_TRUE(tokens.size() == 3);
    }

    TEST(scanner, float_test)
    {
        std::string input = R"(.7e8 5.7e6f64)";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        EXPECT_TRUE(tokens.size() == 3);
    }

    TEST(scanner, test_token_set)
    {
        std::string input = R"(( ) { } [ ] , . + - * / % += -= *= /= %= == != < > <= >= ! = ;)";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        EXPECT_TRUE(tokens.size() == 28);
    }

    TEST(scanner, test_single_line_comment)
    {
        std::string input = R"(// This is a single line comment
var x = 10;)";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        EXPECT_TRUE(tokens.size() == 6);
    }

    TEST(scanner, test_invalid_hex)
    {
        std::string input = R"(0xGHIJ)";
        Scanner scanner(input);
        EXPECT_THROW(scanner.ScanTokens(), star::ScannerException);
    }

    TEST(scanner, test_invalid_float)
    {
        std::string input = R"(5.7e6f64.3)";
        Scanner scanner(input);
        EXPECT_THROW(scanner.ScanTokens(), star::ScannerException);
    }

    TEST(scanner, test_invalid_dotfloat)
    {
        std::string input = R"(.7e6f64.3)";
        Scanner scanner(input);
        EXPECT_THROW(scanner.ScanTokens(), star::ScannerException);
    }

    TEST(scanner, test_invalid_integer)
    {
        std::string input = R"(075u6)";
        Scanner scanner(input);
        EXPECT_THROW(scanner.ScanTokens(), star::ScannerException);
    }

    TEST(scanner, test_invalid_symbol)
    {
        std::string input = R"($)";
        Scanner scanner(input);
        EXPECT_THROW(scanner.ScanTokens(), star::ScannerException);
    }

    TEST(scanner, test_type_attribution)
    {
        std::string input = R"(var x#u32 = 17u32;)";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        EXPECT_EQ(tokens[2].GetTokenType(), TokenType::NUM_SIGN);
    }

    TEST(scanner, test_valid_escaped_chars)
    {
        std::string input = R"(var escaped_str = "this is an escaped pattern \{\}";)";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        EXPECT_EQ(tokens[3].GetLexeme(), R"(this is an escaped pattern {})");
    }

    TEST(scanner, test_invalid_escaped_chars)
    {
        std::string input = R"(var escaped_str = "this is an escaped pattern \{\}\e";)";
        Scanner scanner(input);
        EXPECT_THROW(scanner.ScanTokens(), ScannerException);
    }

    TEST(scanner, test_string_breakline)
    {
        std::string input = R"(var escaped_str = "this is an escaped pattern 
        \{\}\e";)";
        Scanner scanner(input);
        EXPECT_THROW(scanner.ScanTokens(), ScannerException);
    }

    TEST(scanner, test_escape_dbl_quote)
    {
        std::string input = R"(var escaped_str = "this is a dbl quote \" test";)";
        Scanner scanner(input);
        auto tokens = scanner.ScanTokens();
        EXPECT_EQ(tokens[3].GetLexeme(), "this is a dbl quote \" test");
    }

    TEST(scanner, base_exception_test)
    {
        EXPECT_THROW(throw star::ScannerException("element"), star::ScannerException);
    }
}
