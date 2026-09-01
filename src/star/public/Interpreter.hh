#pragma once

#include "Output.hh"
#include "StarMacro.hh"
#include "Token.hh"
#include "Expr.hh"
#include "Stmt.hh"
#include "Visitor.hh"
#include "Environment.hh"
#include <memory>
#include <vector>

namespace star
{
	class STAR_API Interpreter : public Expression::ExprVisitor, Statement::StmtVisitor, public Output
	{
	public:
		Value VisitGroupingExpr(std::shared_ptr<Expression::Grouping> expr) override;
		Value VisitLiteralExpr(std::shared_ptr<Expression::Literal> expr) override;
		Value VisitTemplateLiteralExpr(std::shared_ptr<Expression::TemplateLiteral> expr) override;
		Value VisitUnaryExpr(std::shared_ptr<Expression::Unary> expr) override;
		Value VisitBinaryExpr(std::shared_ptr<Expression::Binary> expr) override;
		Value VisitTernaryExpr(std::shared_ptr<Expression::Ternary> expr) override;
		Value VisitVariableExpr(std::shared_ptr<Expression::Variable> expr) override;
		
		Interpreter();
		virtual ~Interpreter() = default;
		
		void Write(const std::string& text) const override;

		Value Interpret(std::shared_ptr<Expression::Expr> expr);
		Value Interpret(std::vector<std::shared_ptr<Statement::Stmt>>& statements);
		Value ExecuteStmt(std::shared_ptr<Statement::Stmt> statement);

		Value VisitExpressionStmt(std::shared_ptr<Statement::Expression> stmt) override;
        Value VisitPrintStmt(std::shared_ptr<Statement::Print> stmt) override;
		Value VisitVariableStmt(std::shared_ptr<Statement::Variable> stmt) override;
	private:
		void CheckNumberOperand(const Token& oper, const Value& operand);
		void CheckNumberOperands(const Token& oper, const Value& left, const Value& right);
		bool IsTruthy(const Value& object);
		bool IsEqual(const Value& a, const Value& b);
		std::string Stringify(const Value& object);
		Value Evaluate(std::shared_ptr<Expression::Expr> expr);
		std::shared_ptr<Environment> m_GlobalEnv;
	};

}
