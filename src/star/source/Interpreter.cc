#include "Interpreter.hh"
#include "Expr.hh"
#include "Token.hh"
#include "TokenType.hh"
#include "Value.hh"
#include "RuntimeError.hh"
#include "Function.hh"
#include <sstream>
#include <variant>
#include "Escape.hh"

star::Interpreter::Interpreter()
{
	m_CurrentEnv.reset(new Environment());
	m_CurrentEnv->Define(Token{ TokenType::FUN, "escape", 1, 1, "::native" }, Value{ std::make_shared<Escape>() });
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
                return Stringify(i.Interpret(shard.first), shard.second);
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
    Value value = m_CurrentEnv->Get(expr->m_Name);
    if (value.GetType() == VariableType::Null)
    {
        throw RuntimeError(expr->m_Name, "Variable not initialized.");
    }
    return m_CurrentEnv->Get(expr->m_Name);
}

star::Value star::Interpreter::VisitAssignmentExpr(std::shared_ptr<Expression::Assignment> expr)
{
    Value value = Evaluate(expr->m_Value);
    m_CurrentEnv->Reassign(expr->m_Name, value);
    return value;
}

star::Value star::Interpreter::VisitLogicalExpr(std::shared_ptr<Expression::Logical> expr)
{
    Value left = Evaluate(expr->m_Left);
    if (expr->m_Operator.GetTokenType() == TokenType::OR) {
        if (IsTruthy(left)) return left;
    }
    else {
        if (!IsTruthy(left)) return left;
    }
    return Evaluate(expr->m_Right);
}

star::Value star::Interpreter::VisitCallExpr(std::shared_ptr<Expression::Call> expr)
{
    Value callee = Evaluate(expr->m_Callee);
    callee.LockType();
	std::vector<Value> arguments;
    for (auto& argument : expr->m_Arguments)
    {
        arguments.push_back(Evaluate(argument));
    }
    std::shared_ptr<Callable> function;
    if (callee.GetType() == VariableType::Callable)
    {
        if (auto* value = std::get_if<std::shared_ptr<Callable>>(&callee.GetLValue()))
        {
            function = *value;
        }
        else
        {
            throw RuntimeError(expr->m_Paren, "Invalid function value");
        }
		if (function->Arity() != arguments.size())
		{
			throw RuntimeError(expr->m_Paren, "Expected " + std::to_string(function->Arity()) + " arguments but got " + std::to_string(arguments.size()));
        }
    }
    return function->Call(*this, arguments);
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

std::string star::Interpreter::Stringify(const Value& object, const std::string& format)
{
    return object.ToString(format);
}

star::Value star::Interpreter::Evaluate(std::shared_ptr<Expression::Expr> expr)
{
    return expr->Accept(*this);
}

star::Value star::Interpreter::Interpret(std::shared_ptr<Expression::Expr> expr)
{
    return Evaluate(expr);
}

star::Value star::Interpreter::Interpret(std::vector<std::shared_ptr<Statement::Stmt>>& statements)
{
    std::stringstream ss;
    for(auto& stmt: statements)
        ss << ExecuteStmt(stmt) << "\n";
    std::string result = ss.str();
    return {TokenType::NIL, ""};
}

star::Value star::Interpreter::ExecuteStmt(std::shared_ptr<Statement::Stmt> statement)
{
    return statement->Accept(*this);
}

void star::Interpreter::ExecuteBlock
(
    const std::vector<std::shared_ptr<Statement::Stmt>>& statements, 
    std::shared_ptr<Environment> environment
)
{
	std::shared_ptr<Environment> previous = m_CurrentEnv;
    m_CurrentEnv = environment;
    try
    {
        for (auto& stmt : statements)
        {
            ExecuteStmt(stmt);
        }
    }
	catch(...)
	{
		m_CurrentEnv = previous;
		throw;
	}
	
	m_CurrentEnv = previous;
}

star::Value star::Interpreter::VisitExpressionStmt(std::shared_ptr<Statement::Expression> stmt)
{
    Value v = Evaluate(stmt->m_Expression);
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
            m_CurrentEnv->Define(stmt->m_Name, std::move(value));
        }
        else
        {
            value.LockType();
            if(stmt->ExpectedType() != value.GetType())
                throw RuntimeError(stmt->m_Name, "Variable type mismatch.");
			m_CurrentEnv->Define(stmt->m_Name, std::move(value));
        }
    }
    
    return { TokenType::NIL, "" };
}

star::Value star::Interpreter::VisitBlockStmt(std::shared_ptr<Statement::Block> stmt)
{
    ExecuteBlock(stmt->m_Statements, std::make_shared<Environment>(m_CurrentEnv));
    return { TokenType::NIL, "" };
}

star::Value star::Interpreter::VisitIfStmt(std::shared_ptr<Statement::If> stmt)
{
	Value condition = Evaluate(stmt->m_Condition);

	if (IsTruthy(condition))
        return ExecuteStmt(stmt->m_ThenBranch);
    else if (stmt->m_ElseBranch != nullptr)
		return ExecuteStmt(stmt->m_ElseBranch);
    return { TokenType::NIL, "" };
}

star::Value star::Interpreter::VisitWhileStmt(std::shared_ptr<Statement::While> stmt)
{
	Value condition = Evaluate(stmt->m_Condition);
	while (IsTruthy(condition))
	{
		ExecuteStmt(stmt->m_Body);
		condition = Evaluate(stmt->m_Condition);
	}
    return { TokenType::NIL, "" };
}

star::Value star::Interpreter::VisitFunctionStmt(std::shared_ptr<Statement::Function> stmt)
{
    auto function = std::make_shared<Function>(stmt, m_CurrentEnv);
    m_CurrentEnv->Define(stmt->m_Name, { function });
    return { TokenType::NIL, "" };
}

star::Value star::Interpreter::VisitFunctionArgumentStmt(std::shared_ptr<Statement::FunctionArgument> stmt)
{
    return { TokenType::NIL, "" };
}

star::Value star::Interpreter::VisitReturnStmt(std::shared_ptr<Statement::Return> stmt)
{
    Value value = { TokenType::NIL, "" };
    if (stmt->m_Value != nullptr) {
        value = Evaluate(stmt->m_Value);
    }
    throw Returner{ value };
}