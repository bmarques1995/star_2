#pragma once

#include "StarMacro.hh"
#include "TokenType.hh"
#include <cstddef>
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
        friend class Parser;
        friend class ParserException;
    public:
        Token(TokenType type, std::string lexeme, std::any value, size_t line, size_t column, std::string source);
        std::string ToString() const;
    private:
        TokenType m_Type;
        std::string m_Lexeme;
        std::string m_SourceFile;
        std::any m_Literal;
        size_t m_Line;
        size_t m_Column;

        static const std::unordered_map<std::type_index, Printer> s_Printers;
    };
}

STAR_API std::ostream& operator<<(std::ostream& out, const star::Token& token);
