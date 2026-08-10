#pragma once

#include <any>
#include <memory>

namespace star
{
    struct Grouping;
    struct Literal;
    struct TemplateLiteral;
    struct Unary;
    struct Binary;
    struct Ternary;

    struct ExprVisitor
    {
        virtual std::any VisitGroupingExpr(std::shared_ptr<Grouping> expr) = 0;
        virtual std::any VisitLiteralExpr(std::shared_ptr<Literal> expr) = 0;
        virtual std::any VisitTemplateLiteralExpr(std::shared_ptr<TemplateLiteral> expr) = 0;
        virtual std::any VisitUnaryExpr(std::shared_ptr<Unary> expr) = 0;
        virtual std::any VisitBinaryExpr(std::shared_ptr<Binary> expr) = 0;
        virtual std::any VisitTernaryExpr(std::shared_ptr<Ternary> expr) = 0;
        virtual ~ExprVisitor() = default;
    };

    struct Expr
    {
        virtual std::any Accept(ExprVisitor& visitor) = 0;
    };
}
