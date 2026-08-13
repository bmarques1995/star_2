#pragma once
#include "Expr.hh"

namespace star
{
    class STAR_API AstPrinter: public Expression::ExprVisitor
    {
    private:
        template<class... E>
        std::string Parenthesize(std::string_view name, E... expr);
    public:
        std::string Print(std::shared_ptr<Expression::Expr> expr);

        Value VisitBinaryExpr(std::shared_ptr<Expression::Binary> expr);
        Value VisitGroupingExpr(std::shared_ptr<Expression::Grouping> expr);
        Value VisitLiteralExpr(std::shared_ptr<Expression::Literal> expr);
        Value VisitUnaryExpr(std::shared_ptr<Expression::Unary> expr);
        Value VisitTernaryExpr(std::shared_ptr<Expression::Ternary> expr);
        Value VisitTemplateLiteralExpr(std::shared_ptr<Expression::TemplateLiteral> expr);
    };
}
