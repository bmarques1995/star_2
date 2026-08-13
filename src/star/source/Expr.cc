#include "Expr.hh"
#include "Visitor.hh"

star::Expression::Grouping::Grouping(std::shared_ptr<Expr> expression) :
    m_Expression{std::move(expression)}
{
}

star::Value star::Expression::Grouping::Accept(star::Expression::ExprVisitor& visitor)
{
    return visitor.VisitGroupingExpr(shared_from_this());
}

star::Expression::Literal::Literal(Value value) :
    m_Value{std::move(value)}
{
}

star::Value star::Expression::Literal::Accept(star::Expression::ExprVisitor& visitor)
{
    return visitor.VisitLiteralExpr(shared_from_this());
}

star::Expression::TemplateLiteral::TemplateLiteral(const std::vector<TemplateShard>& shards) :
    m_TemplateShards{std::move(shards)}
{
}

star::Value star::Expression::TemplateLiteral::Accept(star::Expression::ExprVisitor& visitor)
{
    return visitor.VisitTemplateLiteralExpr(shared_from_this());
}

star::Expression::Unary::Unary(Token oper, std::shared_ptr<Expr> right) :
    m_Operator{std::move(oper)}, m_Right{std::move(right)}
{
}

star::Value star::Expression::Unary::Accept(star::Expression::ExprVisitor& visitor)
{
    return visitor.VisitUnaryExpr(shared_from_this());
}

star::Expression::Binary::Binary(std::shared_ptr<Expr> left, Token oper, std::shared_ptr<Expr> right) : 
    m_Left{std::move(left)}, m_Operator{std::move(oper)}, m_Right{std::move(right)}
{
}

star::Value star::Expression::Binary::Accept(star::Expression::ExprVisitor& visitor)
{
    return visitor.VisitBinaryExpr(shared_from_this());
}

star::Expression::Ternary::Ternary(std::shared_ptr<Expr> condition, std::shared_ptr<Expr> trueSentence, std::shared_ptr<Expr> falseSentence) :
    m_Condition{std::move(condition)}, 
    m_TrueSentence{std::move(trueSentence)},
    m_FalseSentence{std::move(falseSentence)}
{
}

star::Value star::Expression::Ternary::Accept(star::Expression::ExprVisitor& visitor)
{
    return visitor.VisitTernaryExpr(shared_from_this());
}