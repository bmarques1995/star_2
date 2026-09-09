#pragma once

#include <memory>
#include <vector>
#include "BaseException.hh"
#include "StarMacro.hh"
#include "Token.hh"
#include "Visitor.hh"
#include "Stmt.hh"

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
        std::shared_ptr<Expression::Expr> Assignment();
        std::shared_ptr<Expression::Expr> LogicalOr();
        std::shared_ptr<Expression::Expr> LogicalAnd();
        std::shared_ptr<Expression::Expr> Call();
        std::shared_ptr<Expression::Expr> FinishCall(std::shared_ptr<Expression::Expr> callee);

        std::shared_ptr<Statement::Stmt> Statement();
        std::shared_ptr<Statement::Stmt> PrintStatement();
        std::shared_ptr<Statement::Stmt> ExpressionStatement();
        std::shared_ptr<Statement::Stmt> Declaration();
        std::shared_ptr<Statement::Stmt> VarDeclaration();
        std::shared_ptr<Statement::Stmt> AutoDeclaration();
		std::vector<std::shared_ptr<Statement::Stmt>> Block();
		std::shared_ptr<Statement::Stmt> IfStatement();
		std::shared_ptr<Statement::Stmt> WhileStatement();
        std::shared_ptr<Statement::Stmt> ForStatement();
        //Add expected type
        std::shared_ptr<Statement::Stmt> ReturnStatement();
        //std::shared_ptr<Statement::Stmt> ForeachStatement();
		//std::shared_ptr<Statement::Stmt> SwitchStatement();
        //std::shared_ptr<Statement::Stmt> ThrowStatement();
		//std::shared_ptr<Statement::Stmt> TryStatement();
		//std::shared_ptr<Statement::Stmt> CatchClause();

		std::shared_ptr<Statement::Function> Function(const std::string& name);

        VariableType MatchHashtag();

    public:
        Parser(const std::vector<Token>&);
        ~Parser() = default;
        std::vector<std::shared_ptr<Statement::Stmt>> Parse();
        static const std::unordered_map<std::string, VariableType> s_TypeKeywords;
        static const std::unordered_map<TokenType, TokenType> s_BinaryOperators;
    };
}
