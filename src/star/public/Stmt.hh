#pragma once

#include "StarMacro.hh"
#include "Visitor.hh"
#include <memory>

namespace star
{
    namespace Statement
    {
        struct STAR_API Expression : public Stmt, public std::enable_shared_from_this<Expression>
        {
            std::shared_ptr<star::Expression::Expr> m_Expression;

            Expression(std::shared_ptr<star::Expression::Expr> expression);
            Value Accept(StmtVisitor& visitor) override;
        };

        //Replace with built-in function
        struct STAR_API Print : public Stmt, public std::enable_shared_from_this<Print>
        {
            std::shared_ptr<star::Expression::Expr> m_Expression;

            Print(std::shared_ptr<star::Expression::Expr> expression);
            Value Accept(StmtVisitor& visitor) override;
        };
    }
}