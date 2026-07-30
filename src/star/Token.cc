#include "Token.hh"
#include <magic_enum/magic_enum.hpp>
#include <sstream>

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

star::Token::Token(TokenType type, std::string lexeme, std::any value, uint32_t line) :
    m_Type{type},
    m_Lexeme{lexeme},
    m_Literal{value},
    m_Line{line}
{

}

std::string star::Token::ToString() const
{
    std::stringstream output;
    output << magic_enum::enum_name(this->m_Type).data();
    output << " " << m_Lexeme << " ";
    if(m_Literal.has_value())
    {
        const std::type_index& typeElement = m_Literal.type();
        if (auto it = printers.find(typeElement); it != printers.end())
        {
            it->second(output, m_Literal);
        } 
        else
        {
            output << "null";
        }
    }
    
    else
    {
        output << "[no literal]";
    }

    std::string strToken = output.str();
    return strToken;
}

std::ostream& operator<<(std::ostream& out, const star::Token& token)
{
    out << token.ToString();
    return out;
}