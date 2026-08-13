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

        std::shared_ptr<Expr> Expression();
        std::shared_ptr<Expr> Equality();
        std::shared_ptr<Expr> Comparison();
        std::shared_ptr<Expr> Term();
        std::shared_ptr<Expr> Factor();
        std::shared_ptr<Expr> Unary();
        std::shared_ptr<Expr> Primary();
        std::shared_ptr<Expr> Ternary();
        std::shared_ptr<Expr> TemplateLiteral();

    public:
        Parser(const std::vector<Token>&);
        std::shared_ptr<Expr> Parse();
    };
}
