#pragma once

//REC is for RECURSIVE

namespace star
{
    enum class TokenType
    {
        LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
        COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, MOD, STAR,
        REC_MINUS, REC_PLUS, REC_MOD, REC_SLASH, REC_STAR,
        LEFT_BRACKET, RIGHT_BRACKET,
        QUESTION, COLON, HASHTAG,

        BANG, BANG_EQUAL,
        EQUAL, EQUAL_EQUAL,
        GREATER, GREATER_EQUAL,
        LESS, LESS_EQUAL,

        IDENTIFIER, STRING, NUMBER, FLOAT_NUMBER, DYNAMIC,

        TEMPLATE_STRING_START, TEMPLATE_STRING_END, STR_EXPR_START, STR_EXPR_END, TEMPLATE_SUBSTRING,

        AND, AUTO, CLASS, ELSE, ST_FALSE, FUN, FOR, IF, NIL, OR,
        PRINT, RETURN, SUPER, THIS, ST_TRUE, VAR, WHILE,

        ST_EOF
    };

    enum class FloatType
    {
        F32, F64
    };

    enum class IntegerType
    {
        I8, I16, I32, I64,
        U8, U16, U32, U64
    };
}
