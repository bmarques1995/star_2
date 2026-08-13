#include "Interpreter.hh"
#include "Expr.hh"
#include "Token.hh"
#include "TokenType.hh"
#include "Value.hh"
#include "RuntimeError.hh"
#include <variant>

star::Interpreter::Interpreter() {}

star::Value star::Interpreter::VisitGroupingExpr(std::shared_ptr<Grouping> expr)
{
    return Evaluate(expr->m_Expression);
}

star::Value star::Interpreter::VisitLiteralExpr(std::shared_ptr<Literal> expr)
{
    return expr->m_Value;
}

star::Value star::Interpreter::VisitTemplateLiteralExpr(std::shared_ptr<TemplateLiteral> expr)
{
    std::string v{""};
    auto tokens = expr->m_TemplateShards;
    for(auto it = expr->m_TemplateShards.begin(); it != expr->m_TemplateShards.end(); it++)
    {
        std::string temp = std::visit([](const auto& shard)->std::string
        {
            using T1 = std::decay_t<decltype(shard)>;
            if constexpr (std::is_same_v<T1, std::string>)
            {
                return shard;
            }
            else
            {
                Interpreter i;
                return i.Interpret(shard);
            }
        },
        *it);
        v += temp;
    }
    return {TokenType::STRING, v};
} 

star::Value star::Interpreter::VisitUnaryExpr(std::shared_ptr<Unary> expr)
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

star::Value star::Interpreter::VisitBinaryExpr(std::shared_ptr<Binary> expr)
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
            return {!IsEqual(left, right) ? TokenType::ST_TRUE : TokenType::ST_FALSE, ""};
        default:
            return {TokenType::NIL, ""};
    }
}

star::Value star::Interpreter::VisitTernaryExpr(std::shared_ptr<Ternary> expr)
{
    bool condition;
    try
    {
        condition = std::get<bool>(Evaluate(expr->m_Condition).GetRValue());
    }
    catch(const std::bad_variant_access& e)
    {
        throw InvalidOperation("The condition on a Ternary must be a boolean");
    } 
    return condition ? Evaluate(expr->m_TrueSentence) : Evaluate(expr->m_FalseSentence);
}

bool star::Interpreter::IsTruthy(const Value& object)
{
    using T = std::decay_t<decltype(object)>;

    if constexpr (std::is_same_v<T, std::monostate>) return false;
    if constexpr (std::is_same_v<T, bool>)
    {   
        return std::get<bool>(object.GetRValue());
    }
    
    return true;
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

star::Value star::Interpreter::Evaluate(std::shared_ptr<Expr> expr)
{
    return expr->Accept(*this);
}


std::string star::Interpreter::Interpret(std::shared_ptr<Expr> expr)
{
    Value value = Evaluate(expr);
    return Stringify(value);
}
