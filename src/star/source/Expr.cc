#include "Expr.hh"
#include "Visitor.hh"

star::Grouping::Grouping(std::shared_ptr<Expr> expression) :
    m_Expression{std::move(expression)}
{
}

star::Value star::Grouping::Accept(star::ExprVisitor& visitor)
{
    return visitor.VisitGroupingExpr(shared_from_this());
}

star::Literal::Literal(Value value) :
    m_Value{std::move(value)}
{
}

star::Value star::Literal::Accept(star::ExprVisitor& visitor)
{
    return visitor.VisitLiteralExpr(shared_from_this());
}

star::TemplateLiteral::TemplateLiteral(const std::vector<TemplateShard>& shards) :
    m_TemplateShards{std::move(shards)}
{
}

star::Value star::TemplateLiteral::Accept(star::ExprVisitor& visitor)
{
    return visitor.VisitTemplateLiteralExpr(shared_from_this());
}

star::Unary::Unary(Token oper, std::shared_ptr<Expr> right) :
    m_Operator{std::move(oper)}, m_Right{std::move(right)}
{
}

star::Value star::Unary::Accept(star::ExprVisitor& visitor)
{
    return visitor.VisitUnaryExpr(shared_from_this());
}

star::Binary::Binary(std::shared_ptr<Expr> left, Token oper, std::shared_ptr<Expr> right) : 
    m_Left{std::move(left)}, m_Operator{std::move(oper)}, m_Right{std::move(right)}
{
}

star::Value star::Binary::Accept(star::ExprVisitor& visitor)
{
    return visitor.VisitBinaryExpr(shared_from_this());
}

star::Ternary::Ternary(std::shared_ptr<Expr> condition, std::shared_ptr<Expr> trueSentence, std::shared_ptr<Expr> falseSentence) :
    m_Condition{std::move(condition)}, 
    m_TrueSentence{std::move(trueSentence)},
    m_FalseSentence{std::move(falseSentence)}
{
}

star::Value star::Ternary::Accept(star::ExprVisitor& visitor)
{
    return visitor.VisitTernaryExpr(shared_from_this());
}