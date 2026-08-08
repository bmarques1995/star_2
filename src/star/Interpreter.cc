#include "Interpreter.hh"
#include "RuntimeError.hh"
#include "Token.hh"
#include "TokenType.hh"
#include "Visitor.hh"

star::Interpreter::Interpreter() {}

star::token_lexeme_pair star::Interpreter::VisitLiteralExpr(std::shared_ptr<Literal> expr)
{
  return {expr->m_TokenType, expr->m_Lexeme};
}

star::token_lexeme_pair star::Interpreter::VisitUnaryExpr(std::shared_ptr<Unary> expr){
  token_lexeme_pair right = Evaluate(expr->m_Right);

  switch(expr->m_Operator.GetType()){
    case TokenType::BANG:
      return {(!IsTruthy(right)) ? TokenType::ST_TRUE : TokenType::ST_FALSE, ""};
    case TokenType::MINUS:
      CheckNumberOperand(expr->m_Operator, right);
      return {right.first, ("-" + right.second)};
    default:
      return {TokenType::ST_EMPTY, ""};
  }
}

bool star::Interpreter::IsTruthy(const token_lexeme_pair& object){
    if(object.first == TokenType::NIL) return false;
    if(object.first == TokenType::ST_FALSE) return false;
    if(object.first == TokenType::ST_TRUE) return true;
    return true;
}

void star::Interpreter::CheckNumberOperand(const Token& oper, const token_lexeme_pair& operand){
    if(operand.first == TokenType::NUMBER) return;
    //if(operand.type() == typeid(int)) return;
    //if(operand.type() == typeid(float)) return;
    throw RuntimeError{oper, "Operand must be a number."};
}

void star::Interpreter::CheckNumberOperands(const Token& oper, const token_lexeme_pair& left, const token_lexeme_pair& right)
{
    if(left.first == TokenType::NUMBER && right.first == TokenType::NUMBER) return;
    // verificar float e int
    throw RuntimeError{oper, "Operand must be a number."};
}

bool star::Interpreter::IsEqual(const token_lexeme_pair& a, const token_lexeme_pair& b){
    if(a.first == TokenType::NIL && b.first == TokenType::NIL){
        return true;
    }

    if(a.first == TokenType::NIL || b.first == TokenType::NIL){
        return false;
    }

    /*
    if(a.type() == typeid(double) && b.type() == typeid(double)){
        return std::any_cast<double>(a) == std::any_cast<double>(b);
    }

    if(a.type() == typeid(std::string) && b.type() == typeid(std::string)){
        return std::any_cast<std::string>(a) == std::any_cast<std::string>(b);
    }

    if(a.type() == typeid(bool) && b.type() == typeid(bool)){
        return std::any_cast<bool>(a) == std::any_cast<bool>(b);
    }
    */
    return false;
}

std::string star::Interpreter::Stringify(const token_lexeme_pair& object)
{
    if(object.first == TokenType::NIL) return "nil";
    
    /*
    if(object.type() == typeid(double))
    {
        std::string text = std::to_string(std::any_cast<double>(object));
        if(text[text.length() - 7] == '.' && text[text.length() - 6] == '0')
        {
            text = text.substr(0, text.length() - 7);
        }
        return text;
    }

    if(object.type() == typeid(std::string))
    {
        return std::any_cast<std::string>(object);
    }

    if(object.type() == typeid(bool))
    {
        if(std::any_cast<bool>(object))
        {
            std::string result{"true"};
            return result;
        }
        else
        {
            std::string result{"false"};
            return result;
        }
    }
    */
    return "stringify: cannot reconize type";
}

star::token_lexeme_pair star::Interpreter::VisitGroupingExpr(std::shared_ptr<Grouping> expr)
{
    return Evaluate(expr->m_Expression);
}

star::token_lexeme_pair star::Interpreter::Evaluate(std::shared_ptr<Expr> expr)
{
    return expr->Accept(*this);
}

star::token_lexeme_pair star::Interpreter::VisitBinaryExpr(std::shared_ptr<Binary> expr)
{
    /*
    std::any left = Evaluate(expr->m_Left);
    std::any right = Evaluate(expr->m_Right);

    switch (expr->m_Operator.GetType())
    {
        case TokenType::GREATER:
            CheckNumberOperands(expr->m_Operator, left, right);
            return std::any_cast<double>(left) > std::any_cast<double>(right);
        case TokenType::GREATER_EQUAL:
            CheckNumberOperands(expr->m_Operator, left, right);
            return std::any_cast<double>(left) >= std::any_cast<double>(right);
        case TokenType::LESS:
            CheckNumberOperands(expr->m_Operator, left, right);
            return std::any_cast<double>(left) < std::any_cast<double>(right);
        case TokenType::LESS_EQUAL:
            CheckNumberOperands(expr->m_Operator, left, right);
            return std::any_cast<double>(left) <= std::any_cast<double>(right);
        case TokenType::MINUS:
            CheckNumberOperands(expr->m_Operator, left, right);
            return std::any_cast<double>(left) - std::any_cast<double>(right);
        case TokenType::PLUS:
            if(left.type() == typeid(double) && right.type() == typeid(double)){
                return std::any_cast<double>(left) + std::any_cast<double>(right);
            }

            if(left.type() == typeid(std::string) && right.type() == typeid(std::string)){
                return std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
            }
            throw RuntimeError{expr->m_Operator, "Operands not a same type"};
        case TokenType::STAR:
            CheckNumberOperands(expr->m_Operator, left, right);
            return std::any_cast<double>(left) * std::any_cast<double>(right);
        case TokenType::SLASH:
            CheckNumberOperands(expr->m_Operator, left, right);
            return std::any_cast<double>(left) / std::any_cast<double>(right);
        case TokenType::BANG_EQUAL:
            CheckNumberOperands(expr->m_Operator, left, right);
            return !IsEqual(left, right);
        case TokenType::EQUAL_EQUAL:
            CheckNumberOperands(expr->m_Operator, left, right);
            return !IsEqual(left, right);
        default:
            return {TokenType::ST_EMPTY, ""};
    }*/
    return {TokenType::ST_EMPTY, ""};
}

void star::Interpreter::Interpret(std::shared_ptr<Expr> expr)
{
    auto value = Evaluate(expr);
    std::cout << Stringify(value) << '\n';
}

