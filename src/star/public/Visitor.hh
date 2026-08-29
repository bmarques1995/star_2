#pragma once

#include <memory>
#include "Value.hh"

namespace star
{
    namespace Expression
    {
        struct Grouping;
        struct Literal;
        struct TemplateLiteral;
        struct Unary;
        struct Binary;
        struct Ternary;
        struct Variable;

        struct ExprVisitor
        {
            virtual Value VisitGroupingExpr(std::shared_ptr<Grouping> expr) = 0;
            virtual Value VisitLiteralExpr(std::shared_ptr<Literal> expr) = 0;
            virtual Value VisitTemplateLiteralExpr(std::shared_ptr<TemplateLiteral> expr) = 0;
            virtual Value VisitUnaryExpr(std::shared_ptr<Unary> expr) = 0;
            virtual Value VisitBinaryExpr(std::shared_ptr<Binary> expr) = 0;
            virtual Value VisitTernaryExpr(std::shared_ptr<Ternary> expr) = 0;
            virtual Value VisitVariableExpr(std::shared_ptr<Variable> expr) = 0;
            virtual ~ExprVisitor() = default;
        };

        struct Expr
        {
            virtual Value Accept(ExprVisitor& visitor) = 0;
        };
    }
    namespace Statement
    {
        struct Expression;
        struct Print;
        struct Variable;

        struct StmtVisitor
        {
            virtual Value VisitExpressionStmt(std::shared_ptr<Expression> stmt) = 0;
            virtual Value VisitPrintStmt(std::shared_ptr<Print> stmt) = 0;
            virtual Value VisitVariableStmt(std::shared_ptr<Variable> stmt) = 0;
            virtual ~StmtVisitor() = default;
        };

        struct Stmt
        {
            virtual Value Accept(StmtVisitor& visitor) = 0;
        };
    }
}
