#pragma once

#include <memory>
#include "Value.hh"

namespace star
{
    namespace Expression
    {
        struct Grouping;
        struct Literal;
        struct TemplateLiteral;
        struct Unary;
        struct Binary;
        struct Ternary;
        struct Variable;
        struct Assignment;
        struct Logical;
		struct Call;

        struct ExprVisitor
        {
            virtual Value VisitGroupingExpr(std::shared_ptr<Grouping> expr) = 0;
            virtual Value VisitLiteralExpr(std::shared_ptr<Literal> expr) = 0;
            virtual Value VisitTemplateLiteralExpr(std::shared_ptr<TemplateLiteral> expr) = 0;
            virtual Value VisitUnaryExpr(std::shared_ptr<Unary> expr) = 0;
            virtual Value VisitBinaryExpr(std::shared_ptr<Binary> expr) = 0;
            virtual Value VisitTernaryExpr(std::shared_ptr<Ternary> expr) = 0;
            virtual Value VisitVariableExpr(std::shared_ptr<Variable> expr) = 0;
            virtual Value VisitAssignmentExpr(std::shared_ptr<Assignment> expr) = 0;
			virtual Value VisitLogicalExpr(std::shared_ptr<Logical> expr) = 0;
			virtual Value VisitCallExpr(std::shared_ptr<Call> expr) = 0;
            virtual ~ExprVisitor() = default;
        };

        struct Expr
        {
            virtual Value Accept(ExprVisitor& visitor) = 0;
        };
    }
    namespace Statement
    {
        struct Expression;
        struct Variable;
        struct Block;
        struct If;
        struct While;
        struct Function;
        struct FunctionArgument;
        struct Return;
        //struct Switch;
        //struct Try;
        //struct Throw;
		

        struct StmtVisitor
        {
            virtual Value VisitExpressionStmt(std::shared_ptr<Expression> stmt) = 0;
            virtual Value VisitVariableStmt(std::shared_ptr<Variable> stmt) = 0;
            
            virtual Value VisitBlockStmt(std::shared_ptr<Block> stmt) = 0;
            virtual Value VisitIfStmt(std::shared_ptr<If> stmt) = 0;
            virtual Value VisitWhileStmt(std::shared_ptr<While> stmt) = 0;
            virtual Value VisitFunctionStmt(std::shared_ptr<Function> stmt) = 0;
            virtual Value VisitFunctionArgumentStmt(std::shared_ptr<FunctionArgument> stmt) = 0;
            virtual Value VisitReturnStmt(std::shared_ptr<Return> stmt) = 0;
            virtual ~StmtVisitor() = default;
        };

        struct Stmt
        {
            virtual Value Accept(StmtVisitor& visitor) = 0;
        };
    }
}
