#include "Parser.hh"
#include "Expr.hh"
#include <memory>
#include <sstream>
#define type_assert(E)

star::ParserException::ParserException(const Token& token, const std::string& reason) : 
    ScriptException(),
    m_Token(token)
{
    std::stringstream ss;
    ss << "ParserException: " << reason << " at: " << m_Token;
    m_Reason = ss.str();
}

void star::Parser::Synchronize()
{
    Advance();
    while(!IsAtEnd())
    {
        if(Previous().m_Type == TokenType::SEMICOLON)
            return;
        switch(Peek().m_Type)
        {
            case TokenType::CLASS:
            case TokenType::FUN:
            case TokenType::VAR:
            case TokenType::FOR:
            case TokenType::IF:
            case TokenType::WHILE:
            case TokenType::PRINT:
            case TokenType::RETURN:
                return;
            default:
                Advance();
        }
    }
}

bool star::Parser::IsAtEnd()
{
    return Peek().m_Type == TokenType::ST_EOF;
}

bool star::Parser::Check(const TokenType& type)
{
    if(IsAtEnd())
        return false;
    return Peek().m_Type == type;
}

template<class...T>
bool star::Parser::Match(T...types)
{
    type_assert((... && std::is_same_v<T, TokenType>)); 
    if((... || Check(types))){
        Advance();
        return true;
    }
    return false;
}

star::Token star::Parser::Previous()
{
    if(m_Current == 0)
        return m_Tokens.at(0);
    return m_Tokens.at(m_Current - 1);
}

star::Token star::Parser::Peek()
{
    return m_Tokens.at(m_Current);
}

star::Token star::Parser::Advance()
{
    if(!IsAtEnd())
        m_Current++;
    return Previous();
}

star::Token star::Parser::Consume(const TokenType& token, const std::string& message)
{
    if(Check(token))
        return Advance();
    throw ParserException(Peek(), message);
}

std::shared_ptr<star::Expr> star::Parser::Expression()
{
    return Equality();
}

std::shared_ptr<star::Expr> star::Parser::Equality()
{
    std::shared_ptr<Expr> expr = Comparison();
    while(Match(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL))
    {
        Token op = Previous();
        std::shared_ptr<Expr> right = Comparison();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<star::Expr> star::Parser::Comparison()
{
    std::shared_ptr<Expr> expr = Term();
    while(Match(TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL))
    {
        Token op = Previous();
        std::shared_ptr<Expr> right = Term();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}
std::shared_ptr<star::Expr> star::Parser::Term()
{
    std::shared_ptr<Expr> expr = Factor();
    while(Match(TokenType::PLUS, TokenType::MINUS))
    {
        Token op = Previous();
        std::shared_ptr<Expr> right = Factor();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}
std::shared_ptr<star::Expr> star::Parser::Factor()
{
    std::shared_ptr<Expr> expr = Unary();
    while(Match(TokenType::STAR, TokenType::SLASH, TokenType::MOD))
    {
        Token op = Previous();
        std::shared_ptr<Expr> right = Unary();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}
std::shared_ptr<star::Expr> star::Parser::Unary()
{

    if(Match(TokenType::BANG, TokenType::MINUS))
    {
        Token op = Previous();
        std::shared_ptr<Expr> right = Unary();
        return std::make_shared<star::Unary>(op, right);
    }
    return Primary();
}
std::shared_ptr<star::Expr> star::Parser::Primary()
{
    if(Match(
        TokenType::ST_FALSE,
        TokenType::ST_TRUE,
        TokenType::NIL,
        TokenType::NUMBER,
        TokenType::STRING
    ))
        return std::make_shared<Literal>(Previous().m_Type, Previous().m_Lexeme);

    if(Match(TokenType::LEFT_PAREN))
    {
        std::shared_ptr<Expr> expr = Expression();
        Consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_shared<Grouping>(expr);
    }

    throw ParserException(Peek(), "Expected expression");
}

star::Parser::Parser(const std::vector<star::Token>& tokens) : 
    m_Tokens(tokens), m_Current(0)
{
}

std::shared_ptr<star::Expr> star::Parser::Parse()
{
    return Expression();
}