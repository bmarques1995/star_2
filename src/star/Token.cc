#include "Token.hh"
#include <magic_enum/magic_enum.hpp>
#include <sstream>

star::Token::Token(TokenType type, std::string lexeme, size_t line, size_t column, std::string filepath) :
    m_Type{type},
    m_Lexeme{lexeme},
    m_Line{line},
    m_Column{column},
    m_Filepath{filepath}
{
}

std::string star::Token::ToString() const
{
    std::stringstream output;
    output << magic_enum::enum_name(this->m_Type).data();
    output << " " << m_Lexeme << " at: " << 
    m_Filepath << ":" << m_Line << ":" << m_Column;
    std::string strToken = output.str();
    return strToken;
}

std::ostream& operator<<(std::ostream& out, const star::Token& token)
{
    out << token.ToString();
    return out;
}
