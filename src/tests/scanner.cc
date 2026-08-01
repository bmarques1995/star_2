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
}
