#include "Parser.hh"
#include "Expr.hh"
#include "TokenType.hh"
#include <sstream>
#define inner_assert(E)

star::ParserException::ParserException(const std::string& message)
{
    m_Reason = "[Parser]: " + message;
}

star::Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) 
{

}

std::shared_ptr<star::Expr> star::Parser::Parse()
{
   return Expression(); 
}

std::shared_ptr<star::Expr> star::Parser::Expression()
{
    return Ternary();
}

std::shared_ptr<star::Expr> star::Parser::Equality()
{
    std::shared_ptr<Expr> expr = Comparison();
    while(Match(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL)){
        Token oper = Previous();
        std::shared_ptr<Expr> right = Comparison();
        expr = std::make_shared<Binary>(expr, oper, right);
    }
    return expr;
}

std::shared_ptr<star::Expr> star::Parser::Comparison()
{
    std::shared_ptr<Expr> expr = Term();
    while(Match(TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL))
    {
        Token oper = Previous();
        std::shared_ptr<Expr> right = Term();
        expr = std::make_shared<Binary>(expr, oper, right);
    }
    return expr;
}

std::shared_ptr<star::Expr> star::Parser::Term()
{
    std::shared_ptr<Expr> expr = Factor();
    while(Match(TokenType::MINUS, TokenType::PLUS))
    {
        Token oper = Previous();
        std::shared_ptr<Expr> right = Factor();
        expr = std::make_shared<Binary>(expr, oper, right);
    }
    return expr;
}

std::shared_ptr<star::Expr> star::Parser::Factor()
{
    std::shared_ptr<Expr> expr = Unary();
    while(Match(TokenType::SLASH, TokenType::STAR))
    {
        Token oper = Previous();
        std::shared_ptr<Expr> right = Unary();
        expr = std::make_shared<star::Binary>(expr, oper, right);
    }
    return expr;
}

std::shared_ptr<star::Expr> star::Parser::Unary()
{
    while(Match(TokenType::BANG, TokenType::MINUS))
    {
        Token oper = Previous();
        std::shared_ptr<Expr> right = Unary();
        return std::make_shared<star::Unary>(oper, right);
    }
    return Primary();
}

std::shared_ptr<star::Expr> star::Parser::Primary()
{
    if(Match(TokenType::TEMPLATE_STRING_START))
        return TemplateLiteral();
    if(Match(TokenType::NUMBER, TokenType::STRING, TokenType::FLOAT_NUMBER, 
        TokenType::ST_TRUE, TokenType::ST_FALSE))
    {
        Value v{Previous().GetTokenType(), Previous().GetLexeme()};
        return std::make_shared<star::Literal>(v);
    }

    if(Match(TokenType::LEFT_PAREN))
    {
        std::shared_ptr<Expr> expr = Expression();
        Consume(TokenType::RIGHT_PAREN, "Expected ')' after expression.");
        return std::make_shared<Grouping>(expr);
    }
    
    std::stringstream ss;
    ss << "Expected expression " << Peek().ToString();
    throw ParserException(ss.str());
}

std::shared_ptr<star::Expr> star::Parser::Ternary()
{
    std::shared_ptr<Expr> expr = Equality();

    if (Match(TokenType::QUESTION))
    {
        std::shared_ptr<Expr> thenBranch = Expression();

        Consume(
            TokenType::COLON,
            "Expected ':' after then branch of conditional expression."
        );

        std::shared_ptr<Expr> elseBranch = Expression();

        expr = std::make_shared<star::Ternary>(
            expr,
            thenBranch,
            elseBranch
        );
    }

    return expr;
}

std::shared_ptr<star::Expr> star::Parser::TemplateLiteral()
{
    std::vector<TemplateShard> shards;

    while (!Check(TokenType::TEMPLATE_STRING_END))
    {
        if (Match(TokenType::TEMPLATE_SUBSTRING))
        {
            shards.emplace_back(Previous().GetLexeme());
        }
        else if (Match(TokenType::STR_EXPR_START))
        {
            std::shared_ptr<Expr> expression = Expression();

            Consume(
                TokenType::STR_EXPR_END,
                "Expected '}$' after template expression."
            );

            shards.emplace_back(std::move(expression));
        }
        else
        {
            std::stringstream ss;

            ss << "Expected template substring or expression "
               << Peek().ToString();

            throw ParserException(ss.str());
        }
    }

    Consume(
        TokenType::TEMPLATE_STRING_END,
        "Expected '`' after template string."
    );

    return std::make_shared<star::TemplateLiteral>(shards);
}

template<class...T>
bool star::Parser::Match(T...types)
{
    inner_assert((... && std::is_same_v<T, TokenType>)); 
    if((... || Check(types)))
    {
        Advance();
        return true;
    }
    return false;
}

star::Token star::Parser::Consume(const TokenType& token, const std::string& message)
{
    if(Check(token)) return Advance();
    std::stringstream ss;
    ss << message << Peek().ToString();
    throw ParserException(ss.str());
}

bool star::Parser::Check(const TokenType& type)
{
    if(IsAtEnd()) return false;
    return Peek().m_Type == type;
}

bool star::Parser::IsAtEnd()
{
    return Peek().m_Type == TokenType::ST_EOF;
}

star::Token star::Parser::Advance()
{
    if(!IsAtEnd()) current++;
    return Previous();
}

star::Token star::Parser::Peek()
{
    return tokens.at(current);
}

star::Token star::Parser::Previous()
{
    return tokens.at(current - 1);
}

void star::Parser::Synchronize()
{
    Advance();
    while(!IsAtEnd())
    {
        if(Previous().m_Type == TokenType::SEMICOLON) return;
        switch (Peek().m_Type)
        {
            case TokenType::CLASS:
            case TokenType::FUN:
            case TokenType::VAR:
            case TokenType::FOR:
            case TokenType::IF:
            case TokenType::WHILE:
            case TokenType::PRINT:
            case TokenType::RETURN:
            default:
                return;
        }
    }
    Advance();
}
