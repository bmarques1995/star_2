#include "TokenType.hh"

#include <gtest/gtest.h>
#include <string>
#include "Value.hh"

namespace star
{
    TEST(value, str_type)
    {
        EXPECT_NO_THROW(Value value(TokenType::STRING, "value"));
    }

    TEST(value, float_no_error)
    {
        EXPECT_NO_THROW(Value value(TokenType::FLOAT_NUMBER, ".5e7"));
        EXPECT_NO_THROW(Value value(TokenType::FLOAT_NUMBER, ".5e7f32"));
        EXPECT_NO_THROW(Value value(TokenType::FLOAT_NUMBER, ".5e7f64"));
    }

    TEST(value, int_no_error)
    {
        EXPECT_NO_THROW(Value value(TokenType::NUMBER, "75"));
        EXPECT_NO_THROW(Value value(TokenType::NUMBER, "-75"));
        EXPECT_NO_THROW(Value value(TokenType::NUMBER, "-9223372036854765808"));
        EXPECT_NO_THROW(Value value(TokenType::NUMBER, "18446744073209551615"));
        EXPECT_NO_THROW(Value value(TokenType::NUMBER, "75i8"));
        EXPECT_NO_THROW(Value value(TokenType::NUMBER, "75u8"));
        EXPECT_NO_THROW(Value value(TokenType::NUMBER, "75i16"));
        EXPECT_NO_THROW(Value value(TokenType::NUMBER, "75u16"));
        EXPECT_NO_THROW(Value value(TokenType::NUMBER, "75i32"));
        EXPECT_NO_THROW(Value value(TokenType::NUMBER, "75u32"));
        EXPECT_NO_THROW(Value value(TokenType::NUMBER, "75i64"));
        EXPECT_NO_THROW(Value value(TokenType::NUMBER, "75u64"));
    }

    TEST(value, int_error)
    {
        EXPECT_THROW(Value value(TokenType::NUMBER, "-750i8"), OutOfRangeException);
        EXPECT_THROW(Value value(TokenType::NUMBER, "750i8"), OutOfRangeException);
        EXPECT_THROW(Value value(TokenType::NUMBER, "750u8"), OutOfRangeException);
        EXPECT_THROW(Value value(TokenType::NUMBER, "-9223372036854775908"), OutOfRangeException);
        EXPECT_THROW(Value value(TokenType::NUMBER, "-9223372036854775908i64"), OutOfRangeException);
        EXPECT_THROW(Value value(TokenType::NUMBER, "18446744073749551615"), OutOfRangeException);
        EXPECT_THROW(Value value(TokenType::NUMBER, "18446744073749551615u64"), OutOfRangeException);
    }

    TEST(value, bool_no_error)
    {
        EXPECT_NO_THROW(Value value(TokenType::ST_TRUE, ""));
        EXPECT_NO_THROW(Value value(TokenType::ST_FALSE, ""));
    }

    TEST(value, non_init_element)
    {
        Value value(TokenType::DOT, "");
        EXPECT_FALSE(value.IsInitialized());
    }

    TEST(value, non_recognised_suffixes)
    {
        Value floatTypeError(TokenType::FLOAT_NUMBER,"745f42");
        Value intTypeError(TokenType::NUMBER, "-750i18");
        Value uintTypeError(TokenType::NUMBER, "750u18");
        EXPECT_FALSE(floatTypeError.IsInitialized());
        EXPECT_FALSE(intTypeError.IsInitialized());
        EXPECT_FALSE(uintTypeError.IsInitialized());
    }
}