#include "Token.hh"
#include <magic_enum/magic_enum.hpp>
#include <sstream>
#include <string>

template<typename T>
Printer make_printer()
{
    return [](std::ostream& os, const std::any& a) {
        os << std::any_cast<const T&>(a);
    };
}

static const std::unordered_map<std::type_index, Printer> printers{
    { typeid(std::string), make_printer<std::string>() },
    { typeid(int),         make_printer<int>() },
    { typeid(double),      make_printer<double>() },
};

star::Token::Token(TokenType type, std::string lexeme, 
    size_t line, size_t column, std::string source) :
    m_Type{type},
    m_Lexeme{lexeme},
    m_Line{line},
    m_Column{column},
    m_SourceFile{source}
{

}

std::string star::Token::ToString() const
{
    std::stringstream output;
    output << magic_enum::enum_name(this->m_Type).data();
    output << " " << m_Lexeme << " at: " << m_SourceFile << ", l." << m_Line;
    std::string strToken = output.str();
    return strToken;
}

std::ostream& operator<<(std::ostream& out, const star::Token& token)
{
    out << token.ToString();
    return out;
}

const star::TokenType star::Token::GetType() const
{
    return m_Type;
}
