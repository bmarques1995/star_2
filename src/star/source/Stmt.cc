#include "Stmt.hh"

star::Statement::Expression::Expression(std::shared_ptr<star::Expression::Expr> expression) :
    m_Expression(expression)
{

}

star::Value star::Statement::Expression::Accept(StmtVisitor& visitor)
{
    return visitor.VisitExpressionStmt(shared_from_this());
}

star::Statement::Print::Print(std::shared_ptr<star::Expression::Expr> expression) :
    m_Expression(expression)
{

}

star::Value star::Statement::Print::Accept(StmtVisitor& visitor)
{
    return visitor.VisitPrintStmt(shared_from_this());
}