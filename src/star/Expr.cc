#include "Expr.hh"
#include "Visitor.hh"

star::Binary::Binary(std::shared_ptr<Expr> left, Token oper, std::shared_ptr<Expr> right) : 
    m_Left{std::move(left)}, m_Operator{std::move(oper)}, m_Right{std::move(right)}
{
}

std::any star::Binary::Accept(star::ExprVisitor& visitor)
{
    return visitor.VisitBinaryExpr(shared_from_this());
}

star::Grouping::Grouping(std::shared_ptr<Expr> expression) :
    m_Expression{std::move(expression)}
{
}

std::any star::Grouping::Accept(star::ExprVisitor& visitor)
{
    return visitor.VisitGroupingExpr(shared_from_this());
}

star::Literal::Literal(std::any value) :
    m_Value{std::move(value)}
{
}

std::any star::Literal::Accept(star::ExprVisitor& visitor)
{
    return visitor.VisitLiteralExpr(shared_from_this());
}

star::Unary::Unary(Token oper, std::shared_ptr<Expr> right) :
    m_Operator{std::move(oper)}, m_Right{std::move(right)}
{
}

std::any star::Unary::Accept(star::ExprVisitor& visitor)
{
    return visitor.VisitUnaryExpr(shared_from_this());
}