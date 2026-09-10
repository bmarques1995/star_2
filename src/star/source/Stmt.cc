#include "Stmt.hh"

star::Statement::Expression::Expression(std::shared_ptr<star::Expression::Expr> expression) :
    m_Expression(expression)
{

}

star::Value star::Statement::Expression::Accept(StmtVisitor& visitor)
{
    return visitor.VisitExpressionStmt(shared_from_this());
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

star::Statement::FunctionArgument::FunctionArgument(Token name, VariableType expectedType) :
	m_Name(name), m_ExpectedType(expectedType)
{

}

star::Value star::Statement::FunctionArgument::Accept(StmtVisitor& visitor)
{
	return visitor.VisitFunctionArgumentStmt(shared_from_this());
}

const star::VariableType star::Statement::FunctionArgument::ExpectedType() const
{
	return m_ExpectedType;
}

star::Statement::Block::Block(std::vector<std::shared_ptr<Stmt>> statements) :
	m_Statements(statements)
{}

star::Value star::Statement::Block::Accept(StmtVisitor& visitor)
{
	return visitor.VisitBlockStmt(shared_from_this());
}

star::Statement::If::If(std::shared_ptr<star::Expression::Expr> condition, std::shared_ptr<Stmt> thenBranch, std::shared_ptr<Stmt> elseBranch) :
	m_Condition(condition), m_ThenBranch(thenBranch), m_ElseBranch(elseBranch)
{
}

star::Value star::Statement::If::Accept(StmtVisitor& visitor)
{
    return visitor.VisitIfStmt(shared_from_this());
}

star::Statement::While::While(std::shared_ptr<star::Expression::Expr> condition, std::shared_ptr<Stmt> body) :
	m_Condition(condition), m_Body(body)
{
}

star::Value star::Statement::While::Accept(StmtVisitor& visitor)
{
	return visitor.VisitWhileStmt(shared_from_this());
}

star::Statement::Function::Function(Token name, std::vector<std::shared_ptr<Statement::FunctionArgument>> parameters, 
	std::vector<std::shared_ptr<Stmt>> body, VariableType expectedType) :
	m_Name(name), m_Parameters(parameters), m_Body(body), m_ExpectedType(expectedType)
{
	m_LockType = m_ExpectedType == VariableType::Dynamic ? false : true;
}

star::Value star::Statement::Function::Accept(StmtVisitor& visitor)
{
	return visitor.VisitFunctionStmt(shared_from_this());
}

star::Statement::Return::Return(Token keyword, std::shared_ptr<star::Expression::Expr> value) :
	m_Keyword(keyword), m_Value(value)
{

}

star::Value star::Statement::Return::Accept(StmtVisitor& visitor)
{
	return visitor.VisitReturnStmt(shared_from_this());
}