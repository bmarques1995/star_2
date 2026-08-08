#pragma once

#include "StarMacro.hh"
#include "Token.hh"
#include "Expr.hh"

namespace star
{
    class STAR_API Interpreter : public ExprVisitor
    {
    public:
        std::any VisitBinaryExpr(std::shared_ptr<Binary> expr) override;
        std::any VisitGroupingExpr(std::shared_ptr<Grouping> expr) override;
        std::any VisitLiteralExpr(std::shared_ptr<Literal> expr) override;
        std::any VisitUnaryExpr(std::shared_ptr<Unary> expr) override;
        Interpreter();
        void Interpret(std::shared_ptr<Expr> expr);

    private:
        void CheckNumberOperand(const Token& oper, const std::any& operand);
        void CheckNumberOperands(const Token& oper, const std::any& left, const std::any& right);
        bool IsTruthy(const std::any& object);
        bool IsEqual(const std::any& a, const std::any& b);
        std::string Stringify(const std::any& object);
        std::any Evaluate(std::shared_ptr<Expr> expr);
    };
}
