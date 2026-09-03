#include "Interpreter.hh"
#include "Expr.hh"
#include "Token.hh"
#include "TokenType.hh"
#include "Value.hh"
#include "RuntimeError.hh"
#include <sstream>
#include <variant>

star::Interpreter::Interpreter()
{
	m_GlobalEnv.reset(new Environment());
}

star::Value star::Interpreter::VisitGroupingExpr(std::shared_ptr<Expression::Grouping> expr)
{
    return Evaluate(expr->m_Expression);
}

star::Value star::Interpreter::VisitLiteralExpr(std::shared_ptr<Expression::Literal> expr)
{
    return expr->m_Value;
}

star::Value star::Interpreter::VisitTemplateLiteralExpr(std::shared_ptr<Expression::TemplateLiteral> expr)
{
    std::string v{""};
    auto tokens = expr->m_TemplateShards;
    for(auto it = expr->m_TemplateShards.begin(); it != expr->m_TemplateShards.end(); it++)
    {
        std::string temp = std::visit([this](const auto& shard)->std::string
        {
            using T1 = std::decay_t<decltype(shard)>;
            if constexpr (std::is_same_v<T1, std::string>)
            {
                return shard;
            }
            else
            {
                Interpreter i;
                return Stringify(i.Interpret(shard));
            }
        },
        *it);
        v += temp;
    }
    return {TokenType::STRING, v};
} 

star::Value star::Interpreter::VisitUnaryExpr(std::shared_ptr<Expression::Unary> expr)
{
    Value right = Evaluate(expr->m_Right);

    switch(expr->m_Operator.GetTokenType())
    {
        case TokenType::BANG:
            return {!IsTruthy(right) ? TokenType::ST_TRUE : TokenType::ST_FALSE, ""};
        case TokenType::MINUS:
            CheckNumberOperand(expr->m_Operator, right);
            return -right;
        default:
            return {TokenType::NIL, ""};
    }
}

star::Value star::Interpreter::VisitBinaryExpr(std::shared_ptr<Expression::Binary> expr)
{
    Value left = Evaluate(expr->m_Left);
    Value right = Evaluate(expr->m_Right);

    switch (expr->m_Operator.GetTokenType()) 
    {
        case TokenType::GREATER:
            CheckNumberOperands(expr->m_Operator, left, right);
            return {left > right ? TokenType::ST_TRUE : TokenType::ST_FALSE, ""};
        case TokenType::GREATER_EQUAL:
            CheckNumberOperands(expr->m_Operator, left, right);
            return {left >= right ? TokenType::ST_TRUE : TokenType::ST_FALSE, ""};
        case TokenType::LESS:
            CheckNumberOperands(expr->m_Operator, left, right);
            return {left < right ? TokenType::ST_TRUE : TokenType::ST_FALSE, ""};
        case TokenType::LESS_EQUAL:
            CheckNumberOperands(expr->m_Operator, left, right);
            //return std::any_cast<double>(left) <= std::any_cast<double>(right);
            return {left <= right ? TokenType::ST_TRUE : TokenType::ST_FALSE, ""};
        case TokenType::MINUS:
            CheckNumberOperands(expr->m_Operator, left, right);
            return left - right;
        case TokenType::PLUS:
            try
            {
                return left + right;
            }
            catch(const InvalidOperation& e)
            {
                throw RuntimeError{expr->m_Operator, e.what()};
            }
        case TokenType::STAR:
            CheckNumberOperands(expr->m_Operator, left, right);
            return left * right;
        case TokenType::SLASH:
            CheckNumberOperands(expr->m_Operator, left, right);
            return left / right;
        case TokenType::MOD:
            CheckNumberOperands(expr->m_Operator, left, right);
            return left % right;
        case TokenType::BANG_EQUAL:
            CheckNumberOperands(expr->m_Operator, left, right);
            return {!IsEqual(left, right) ? TokenType::ST_TRUE : TokenType::ST_FALSE, ""};
        case TokenType::EQUAL_EQUAL:
            CheckNumberOperands(expr->m_Operator, left, right);
            return {IsEqual(left, right) ? TokenType::ST_TRUE : TokenType::ST_FALSE, ""};
        default:
            return {TokenType::NIL, ""};
    }
}

star::Value star::Interpreter::VisitTernaryExpr(std::shared_ptr<Expression::Ternary> expr)
{
    return std::visit([this, expr](const auto& shard)-> Value
        {
            using T = std::decay_t<decltype(shard)>;
            bool condition;
            if constexpr (std::is_same_v<T, bool>)
            {
                condition = shard;
            }
            else
            {
                throw InvalidOperation("The condition on a Ternary must be a boolean");
            } 
            return condition ? Evaluate(expr->m_TrueSentence) : Evaluate(expr->m_FalseSentence);
        },
        Evaluate(expr->m_Condition).GetRValue());
}

star::Value star::Interpreter::VisitVariableExpr(std::shared_ptr<Expression::Variable> expr)
{
    Value value = m_GlobalEnv->Get(expr->m_Name);
    if (value.GetType() == VariableType::Null)
    {
        throw RuntimeError(expr->m_Name, "Variable not initialized.");
    }
    return m_GlobalEnv->Get(expr->m_Name);
}

star::Value star::Interpreter::VisitAssignmentExpr(std::shared_ptr<Expression::Assignment> expr)
{
    Value value = Evaluate(expr->m_Value);
    m_GlobalEnv->Reassign(expr->m_Name, value);
    return value;
}

bool star::Interpreter::IsTruthy(const Value& object)
{
    return std::visit([](const auto& shard)-> bool
        {
            using T = std::decay_t<decltype(shard)>;
            if constexpr (std::is_same_v<T, std::monostate>) return false;
            if constexpr (std::is_same_v<T, bool>)
                return shard;
            else 
                return true;
        },
        object.GetRValue());
}

void star::Interpreter::CheckNumberOperand(const Token& oper, const Value& operand)
{
    if(operand.IsNumber()) return;
    throw RuntimeError{oper, "Operand must be a number."};
}

void star::Interpreter::CheckNumberOperands(const Token& oper, const Value& left, const Value& right)
{
    if(left.IsNumber() && right.IsNumber()) return;
    throw RuntimeError{oper, "Operand must be a number."};
}

bool star::Interpreter::IsEqual(const Value& a, const Value& b)
{
    return a == b;
}

std::string star::Interpreter::Stringify(const Value& object)
{
    return object.ToString();
}

star::Value star::Interpreter::Evaluate(std::shared_ptr<Expression::Expr> expr)
{
    return expr->Accept(*this);
}

void star::Interpreter::Write(const std::string& text) const
{

}

star::Value star::Interpreter::Interpret(std::shared_ptr<Expression::Expr> expr)
{
    return Evaluate(expr);
}

star::Value star::Interpreter::Interpret(std::vector<std::shared_ptr<Statement::Stmt>>& statements)
{
    std::stringstream ss;
    for(auto stmt: statements)
        ss << ExecuteStmt(stmt) << "\n";
    std::string result = ss.str();
    return {TokenType::NIL, ""};
}

star::Value star::Interpreter::ExecuteStmt(std::shared_ptr<Statement::Stmt> statement)
{
    return statement->Accept(*this);
}

star::Value star::Interpreter::VisitExpressionStmt(std::shared_ptr<Statement::Expression> stmt)
{
    Value v = Evaluate(stmt->m_Expression);
    return {TokenType::NIL, ""};
}

star::Value star::Interpreter::VisitPrintStmt(std::shared_ptr<Statement::Print> stmt)
{
    Value v = Evaluate(stmt->m_Expression);
    Write(Stringify(v));
    return {TokenType::NIL, ""};
}

star::Value star::Interpreter::VisitVariableStmt(std::shared_ptr<Statement::Variable> stmt)
{
    if (stmt->m_Init != nullptr) {
        Value value = Evaluate(stmt->m_Init);
        if (stmt->ExpectedType() == VariableType::Dynamic)
        {
            if(stmt->m_LockType)
				value.LockType();
            m_GlobalEnv->Define(stmt->m_Name, std::move(value));
        }
        else
        {
            value.LockType();
            if(stmt->ExpectedType() != value.GetType())
                throw RuntimeError(stmt->m_Name, "Variable type mismatch.");
			m_GlobalEnv->Define(stmt->m_Name, std::move(value));
        }
    }
    
    return { TokenType::NIL, "" };
}
