#pragma once

#include <any>
#include <memory>

namespace star
{
    struct Binary;
    struct Grouping;
    struct Literal;
    struct Unary;

    struct ExprVisitor
    {
        virtual std::any VisitBinaryExpr(std::shared_ptr<Binary> expr) = 0;
        virtual std::any VisitGroupingExpr(std::shared_ptr<Grouping> expr) = 0;
        virtual std::any VisitLiteralExpr(std::shared_ptr<Literal> expr) = 0;
        virtual std::any VisitUnaryExpr(std::shared_ptr<Unary> expr) = 0;
        virtual ~ExprVisitor() = default;
    };

    struct Expr
    {
        virtual std::any Accept(ExprVisitor& visitor) = 0;
    };
}
