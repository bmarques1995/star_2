#pragma once

#include "Token.hh"
#include "Visitor.hh"
#include <memory>
#include "StarMacro.hh"

namespace star
{
    struct STAR_API Binary final : public Expr, public std::enable_shared_from_this<Binary>
    {
        std::shared_ptr<Expr> m_Left;
        Token m_Operator;
        std::shared_ptr<Expr> m_Right;

        Binary(std::shared_ptr<Expr> left, Token oper, std::shared_ptr<Expr> right);
        ~Binary() = default;
        std::any Accept(ExprVisitor& visitor) override;
    };

    struct STAR_API Grouping final : public Expr, public std::enable_shared_from_this<Grouping>
    {
        std::shared_ptr<Expr> m_Expression;

        Grouping(std::shared_ptr<Expr> expression);
        ~Grouping() = default;
        std::any Accept(ExprVisitor& visitor) override;
    };

    struct STAR_API Literal final : public Expr, public std::enable_shared_from_this<Literal>
    {
        std::any m_Value;

        Literal(std::any value);
        ~Literal() = default;
        std::any Accept(ExprVisitor& visitor) override;
    };

    struct STAR_API Unary final : public Expr, public std::enable_shared_from_this<Unary>
    {
        Token m_Operator;
        std::shared_ptr<Expr> m_Right;

        Unary(Token oper, std::shared_ptr<Expr> right);
        ~Unary() = default;
        std::any Accept(ExprVisitor& visitor) override;
    };
}
