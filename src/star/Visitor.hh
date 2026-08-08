#pragma once

#include "TokenType.hh"
#include <utility>
#include <string>
#include <memory>

namespace star
{
    struct Binary;
    struct Grouping;
    struct Literal;
    struct Unary;

    using token_lexeme_pair = std::pair<star::TokenType, std::string>; 

    struct ExprVisitor
    {
        virtual token_lexeme_pair VisitBinaryExpr(std::shared_ptr<Binary> expr) = 0;
        virtual token_lexeme_pair VisitGroupingExpr(std::shared_ptr<Grouping> expr) = 0;
        virtual token_lexeme_pair VisitLiteralExpr(std::shared_ptr<Literal> expr) = 0;
        virtual token_lexeme_pair VisitUnaryExpr(std::shared_ptr<Unary> expr) = 0;
        virtual ~ExprVisitor() = default;
    };

    struct Expr
    {
        virtual token_lexeme_pair Accept(ExprVisitor& visitor) = 0;
    };
}
