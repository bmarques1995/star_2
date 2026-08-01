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

    TEST(scanner, correct_multiline_string)
    {
        std::string input = "var lang = \"something\nat two lines\";";
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
        EXPECT_TRUE(tokens.size() > 1);
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
}
