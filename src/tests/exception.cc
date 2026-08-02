#include <gtest/gtest.h>
#include <string>
#include "BaseException.hh"

namespace star
{
    TEST(base_exception, default_constructor)
    {
        star::ScriptException exception;
        EXPECT_EQ(exception.what(), std::string(""));
    }
}
