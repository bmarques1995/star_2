#pragma once

#include "Token.hh"
#include "Expr.hh"

namespace star
{
	class Interpreter : public ExprVisitor 
	{
	public:
		Value VisitGroupingExpr(std::shared_ptr<Grouping> expr) override;
		Value VisitLiteralExpr(std::shared_ptr<Literal> expr) override;
		Value VisitTemplateLiteralExpr(std::shared_ptr<TemplateLiteral> expr) override;
		Value VisitUnaryExpr(std::shared_ptr<Unary> expr) override;
		Value VisitBinaryExpr(std::shared_ptr<Binary> expr) override;
		Value VisitTernaryExpr(std::shared_ptr<Ternary> expr) override;
		Interpreter();
		std::string Interpret(std::shared_ptr<Expr> expr);

	private:
		void CheckNumberOperand(const Token& oper, const Value& operand);
		void CheckNumberOperands(const Token& oper, const Value& left, const Value& right);
		bool IsTruthy(const Value& object);
		bool IsEqual(const Value& a, const Value& b);
		std::string Stringify(const Value& object);
		Value Evaluate(std::shared_ptr<Expr> expr);
	};

}
