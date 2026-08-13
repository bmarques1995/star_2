#pragma once
#include "Expr.hh"

namespace star
{
    class STAR_API AstPrinter: public ExprVisitor
    {
    private:
        template<class... E>
        std::string Parenthesize(std::string_view name, E... expr);
    public:
        std::string Print(std::shared_ptr<Expr> expr);

        Value VisitBinaryExpr(std::shared_ptr<Binary> expr);
        Value VisitGroupingExpr(std::shared_ptr<Grouping> expr);
        Value VisitLiteralExpr(std::shared_ptr<Literal> expr);
        Value VisitUnaryExpr(std::shared_ptr<Unary> expr);
        Value VisitTernaryExpr(std::shared_ptr<Ternary> expr);
        Value VisitTemplateLiteralExpr(std::shared_ptr<TemplateLiteral> expr);
    };
}
