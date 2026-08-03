#pragma once

#include "StarMacro.hh"
#include "BaseException.hh"
#include <vector>
#include "Token.hh"
#include "Visitor.hh"

namespace star
{
    class STAR_API ParserException final : public ScriptException, public std::runtime_error
    {
    public:
        ParserException(const Token& token, const std::string& reason);
        ~ParserException() = default;
    private:
        const Token m_Token;
    };

    class STAR_API Parser
    {
    private:
        const std::vector<Token> m_Tokens;
        size_t m_Current;

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
    
    public:
        Parser(const std::vector<Token>&);
        ~Parser() = default;
    
        std::shared_ptr<Expr> Parse();
    };
}