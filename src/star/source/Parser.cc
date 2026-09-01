#include "Parser.hh"
#include "Expr.hh"
#include "TokenType.hh"
#include "Visitor.hh"
#include <memory>
#include <sstream>
#define inner_assert(E)

star::ParserException::ParserException(const std::string& message)
{
    m_Reason = "[Parser]: " + message;
}

star::Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) 
{

}

std::vector<std::shared_ptr<star::Statement::Stmt>> star::Parser::Parse()
{
    std::vector<std::shared_ptr<star::Statement::Stmt>> statements;
    while (!IsAtEnd())
    {
        statements.push_back(Declaration());
    }
    return statements;
}

std::shared_ptr<star::Expression::Expr> star::Parser::Expression()
{
    return Ternary();
}

std::shared_ptr<star::Expression::Expr> star::Parser::Equality()
{
    std::shared_ptr<Expression::Expr> expr = Comparison();
    while(Match(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL)){
        Token oper = Previous();
        std::shared_ptr<Expression::Expr> right = Comparison();
        expr = std::make_shared<Expression::Binary>(expr, oper, right);
    }
    return expr;
}

std::shared_ptr<star::Expression::Expr> star::Parser::Comparison()
{
    std::shared_ptr<Expression::Expr> expr = Term();
    while(Match(TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL))
    {
        Token oper = Previous();
        std::shared_ptr<Expression::Expr> right = Term();
        expr = std::make_shared<Expression::Binary>(expr, oper, right);
    }
    return expr;
}

std::shared_ptr<star::Expression::Expr> star::Parser::Term()
{
    std::shared_ptr<Expression::Expr> expr = Factor();
    while(Match(TokenType::MINUS, TokenType::PLUS))
    {
        Token oper = Previous();
        std::shared_ptr<Expression::Expr> right = Factor();
        expr = std::make_shared<Expression::Binary>(expr, oper, right);
    }
    return expr;
}

std::shared_ptr<star::Expression::Expr> star::Parser::Factor()
{
    std::shared_ptr<Expression::Expr> expr = Unary();
    while(Match(TokenType::SLASH, TokenType::STAR, TokenType::MOD))
    {
        Token oper = Previous();
        std::shared_ptr<Expression::Expr> right = Unary();
        expr = std::make_shared<star::Expression::Binary>(expr, oper, right);
    }
    return expr;
}

std::shared_ptr<star::Expression::Expr> star::Parser::Unary()
{
    while(Match(TokenType::BANG, TokenType::MINUS))
    {
        Token oper = Previous();
        std::shared_ptr<Expression::Expr> right = Unary();
        return std::make_shared<star::Expression::Unary>(oper, right);
    }
    return Primary();
}

std::shared_ptr<star::Expression::Expr> star::Parser::Primary()
{
    if(Match(TokenType::IDENTIFIER))
        return std::make_shared<star::Expression::Variable>(Previous());
    if(Match(TokenType::TEMPLATE_STRING_START))
        return TemplateLiteral();
    if(Match(TokenType::NUMBER, TokenType::STRING, TokenType::FLOAT_NUMBER, 
        TokenType::ST_TRUE, TokenType::ST_FALSE))
    {
        Value v{Previous().GetTokenType(), Previous().GetLexeme()};
        return std::make_shared<star::Expression::Literal>(v);
    }

    if(Match(TokenType::LEFT_PAREN))
    {
        std::shared_ptr<Expression::Expr> expr = Expression();
        Consume(TokenType::RIGHT_PAREN, "Expected ')' after expression.");
        return std::make_shared<Expression::Grouping>(expr);
    }
    
    std::stringstream ss;
    ss << "Expected expression " << Peek().ToString();
    throw ParserException(ss.str());
}

std::shared_ptr<star::Expression::Expr> star::Parser::Ternary()
{
    std::shared_ptr<Expression::Expr> expr = Equality();

    if (Match(TokenType::QUESTION))
    {
        std::shared_ptr<Expression::Expr> thenBranch = Expression();

        Consume(
            TokenType::COLON,
            "Expected ':' after then branch of conditional expression."
        );

        std::shared_ptr<Expression::Expr> elseBranch = Expression();

        expr = std::make_shared<star::Expression::Ternary>(
            expr,
            thenBranch,
            elseBranch
        );
    }

    return expr;
}

std::shared_ptr<star::Expression::Expr> star::Parser::TemplateLiteral()
{
    std::vector<Expression::TemplateShard> shards;

    while (!Check(TokenType::TEMPLATE_STRING_END))
    {
        if (Match(TokenType::TEMPLATE_SUBSTRING))
        {
            shards.emplace_back(Previous().GetLexeme());
        }
        else if (Match(TokenType::STR_EXPR_START))
        {
            std::shared_ptr<Expression::Expr> expression = Expression();

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

    return std::make_shared<star::Expression::TemplateLiteral>(shards);
}

std::shared_ptr<star::Statement::Stmt> star::Parser::Statement()
{
    if(Match(TokenType::PRINT)) return PrintStatement();
    else return ExpressionStatement();
}

std::shared_ptr<star::Statement::Stmt> star::Parser::PrintStatement()
{
    std::shared_ptr<Expression::Expr> value = Expression();
    Consume(TokenType::SEMICOLON, "Expected ; after value.");
    return std::make_shared<Statement::Print>(value);
}

std::shared_ptr<star::Statement::Stmt> star::Parser::ExpressionStatement()
{
    std::shared_ptr<Expression::Expr> expr = Expression();
    Consume(TokenType::SEMICOLON, "Expected ; after value.");
    return std::make_shared<Statement::Expression>(expr);
}

std::shared_ptr<star::Statement::Stmt> star::Parser::Declaration()
{
    try
    {
        if(Match(TokenType::VAR)) return VarDeclaration();
		else return Statement();
    }
    catch (const std::exception& e)
    {
        Synchronize();
        return nullptr;
    }
}

std::shared_ptr<star::Statement::Stmt> star::Parser::VarDeclaration()
{
    Token name = Consume(TokenType::IDENTIFIER, "Expected variable name.");

	std::shared_ptr<Expression::Expr> init = nullptr;
	if(Match(TokenType::EQUAL))
	{
		init = Expression();
	}
	Consume(TokenType::SEMICOLON, "Expected ; after variable declaration.");
	return std::make_shared<Statement::Variable>(name, init);
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
