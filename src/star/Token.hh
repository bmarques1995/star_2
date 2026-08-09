#pragma once

#include "StarMacro.hh"
#include "TokenType.hh"
#include <string>
#include <iostream>
#include <any>
#include <functional>
#include <string>
#include <typeindex>
#include <unordered_map>

#ifdef STAR_DEBUG
using Printer = std::function<void(std::ostream&, const std::any&)>;
#endif

namespace star
{
    class STAR_API Token
    {
    public:
        Token(TokenType type, std::string lexeme, size_t line, size_t column, std::string filepath);
        std::string ToString() const;

    private:
        TokenType m_Type;
        std::string m_Lexeme;
        std::string m_Filepath;
        size_t m_Line;
        size_t m_Column;
    };
}

STAR_API std::ostream& operator<<(std::ostream& out, const star::Token& token);
