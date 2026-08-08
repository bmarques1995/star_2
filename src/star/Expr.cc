#include "Expr.hh"
#include "Visitor.hh"
#include <utility>

star::Binary::Binary(std::shared_ptr<Expr> left, Token oper, std::shared_ptr<Expr> right) : 
    m_Left{std::move(left)}, m_Operator{std::move(oper)}, m_Right{std::move(right)}
{
}

star::token_lexeme_pair star::Binary::Accept(star::ExprVisitor& visitor)
{
    return visitor.VisitBinaryExpr(shared_from_this());
}

star::Grouping::Grouping(std::shared_ptr<Expr> expression) :
    m_Expression{std::move(expression)}
{
}

star::token_lexeme_pair star::Grouping::Accept(star::ExprVisitor& visitor)
{
    return visitor.VisitGroupingExpr(shared_from_this());
}

star::Literal::Literal(TokenType tokenType,std::string lexeme) :
    m_TokenType{std::move(tokenType)}, m_Lexeme{std::move(lexeme)}
{
}

star::token_lexeme_pair star::Literal::Accept(star::ExprVisitor& visitor)
{
    return visitor.VisitLiteralExpr(shared_from_this());
}

star::Unary::Unary(Token oper, std::shared_ptr<Expr> right) :
    m_Operator{std::move(oper)}, m_Right{std::move(right)}
{
}

star::token_lexeme_pair star::Unary::Accept(star::ExprVisitor& visitor)
{
    return visitor.VisitUnaryExpr(shared_from_this());
}