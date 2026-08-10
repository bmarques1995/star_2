#pragma once

#include "StarMacro.hh"
#include "TokenType.hh"
#include <string>
#include <iostream>
#include <string>


namespace star
{
    class STAR_API Token
    {
    public:
        Token(TokenType type, std::string lexeme, size_t line, size_t column, std::string filepath);
        std::string ToString() const;
        const TokenType GetTokenType() const;
        const std::string& GetLexeme() const;

    private:
        TokenType m_Type;
        std::string m_Lexeme;
        std::string m_Filepath;
        size_t m_Line;
        size_t m_Column;
    };
}

STAR_API std::ostream& operator<<(std::ostream& out, const star::Token& token);
