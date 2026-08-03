#include <gtest/gtest.h>
#include <string>
#include "Scanner.hh"

namespace star
{
    TEST(scanner_exception, default_constructor)
    {
        star::ScannerException exception("Symbol not recognized", 1);
        EXPECT_EQ(exception.what(), std::string("ScannerException: Symbol not recognized at line: 1"));
    }
}
