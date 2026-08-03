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
        friend class Parser;
    public:
        Token(TokenType type, std::string lexeme, std::any value, size_t line);
#ifdef STAR_DEBUG
        std::string ToString() const;
#endif
    private:
        TokenType m_Type;
        std::string m_Lexeme;
        std::any m_Literal;
        size_t m_Line;
#ifdef STAR_DEBUG
        static const std::unordered_map<std::type_index, Printer> s_Printers;
#endif
    };
}

#ifdef STAR_DEBUG
STAR_API std::ostream& operator<<(std::ostream& out, const star::Token& token);
#endif
