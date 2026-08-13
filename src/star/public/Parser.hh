#pragma once

#include <memory>
#include <vector>
#include "BaseException.hh"
#include "StarMacro.hh"
#include "Token.hh"
#include "Visitor.hh"

namespace star
{
    class STAR_API ParserException : public ScriptException
    {
    public:
        ParserException(const std::string& message);
        ~ParserException() = default;
    };

    class STAR_API Parser
    {
    private:
        const std::vector<Token> tokens;
        size_t current = 0;

        void Synchronize();

        bool IsAtEnd();
        bool Check(const TokenType&);
        template<class...T>
        bool Match(T...types);

        Token Previous();
        Token Peek();
        Token Advance();
        Token Consume(const TokenType&, const std::string&);

        std::shared_ptr<Expression::Expr> Expression();
        std::shared_ptr<Expression::Expr> Equality();
        std::shared_ptr<Expression::Expr> Comparison();
        std::shared_ptr<Expression::Expr> Term();
        std::shared_ptr<Expression::Expr> Factor();
        std::shared_ptr<Expression::Expr> Unary();
        std::shared_ptr<Expression::Expr> Primary();
        std::shared_ptr<Expression::Expr> Ternary();
        std::shared_ptr<Expression::Expr> TemplateLiteral();

    public:
        Parser(const std::vector<Token>&);
        ~Parser() = default;
        std::shared_ptr<Expression::Expr> Parse();
    };
}
