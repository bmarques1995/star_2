#pragma once

#include <any>
#include <memory>
#include "Value.hh"

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
        virtual Value VisitGroupingExpr(std::shared_ptr<Grouping> expr) = 0;
        virtual Value VisitLiteralExpr(std::shared_ptr<Literal> expr) = 0;
        virtual Value VisitTemplateLiteralExpr(std::shared_ptr<TemplateLiteral> expr) = 0;
        virtual Value VisitUnaryExpr(std::shared_ptr<Unary> expr) = 0;
        virtual Value VisitBinaryExpr(std::shared_ptr<Binary> expr) = 0;
        virtual Value VisitTernaryExpr(std::shared_ptr<Ternary> expr) = 0;
        virtual ~ExprVisitor() = default;
    };

    struct Expr
    {
        virtual Value Accept(ExprVisitor& visitor) = 0;
    };
}
