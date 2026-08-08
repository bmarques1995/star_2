#pragma once

#include "StarMacro.hh"
#include "Token.hh"
#include "Expr.hh"

namespace star
{
    class STAR_API Interpreter : public ExprVisitor
    {
    public:
        token_lexeme_pair VisitBinaryExpr(std::shared_ptr<Binary> expr) override;
        token_lexeme_pair VisitGroupingExpr(std::shared_ptr<Grouping> expr) override;
        token_lexeme_pair VisitLiteralExpr(std::shared_ptr<Literal> expr) override;
        token_lexeme_pair VisitUnaryExpr(std::shared_ptr<Unary> expr) override;
        Interpreter();
        void Interpret(std::shared_ptr<Expr> expr);

    private:
        void CheckNumberOperand(const Token& oper, const token_lexeme_pair& operand);
        void CheckNumberOperands(const Token& oper, const token_lexeme_pair& left, const token_lexeme_pair& right);
        bool IsTruthy(const token_lexeme_pair& object);
        bool IsEqual(const token_lexeme_pair& a, const token_lexeme_pair& b);
        std::string Stringify(const token_lexeme_pair& object);
        token_lexeme_pair Evaluate(std::shared_ptr<Expr> expr);
    };
}
