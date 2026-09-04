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

star::Statement::Variable::Variable(Token name, std::shared_ptr<star::Expression::Expr> init, VariableType expectedType) :
	m_Name(name), m_Init(init), m_ExpectedType(expectedType)
{
    if (expectedType == VariableType::Dynamic) m_LockType = false;
    else m_LockType = true;
}

star::Statement::Variable::Variable(Token name, std::shared_ptr<star::Expression::Expr> init, bool lockType) :
    m_Name(name), m_Init(init), m_ExpectedType(VariableType::Dynamic), m_LockType(lockType)
{}

star::Value star::Statement::Variable::Accept(StmtVisitor& visitor)
{
    return visitor.VisitVariableStmt(shared_from_this());
}



const star::VariableType star::Statement::Variable::ExpectedType() const
{
    return m_ExpectedType;
}