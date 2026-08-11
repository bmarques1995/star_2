#include "AstPrinter.hh"
#include "Expr.hh"
#include "Value.hh"
#include <memory>
#include <sstream>
#define inner_assert(E) // REMOVER DEPOIS

std::string star::AstPrinter::Print(std::shared_ptr<Expr> expr)
{
    return expr->Accept(*this).ToString();
}

star::Value star::AstPrinter::VisitBinaryExpr(std::shared_ptr<Binary> expr)
{
    return Value{TokenType::STRING,
        Parenthesize(expr->m_Operator.GetLexeme(), expr->m_Left, expr->m_Right)};
}

star::Value star::AstPrinter::VisitGroupingExpr(std::shared_ptr<Grouping> expr)
{
    return Value{TokenType::STRING,
        Parenthesize("group", expr->m_Expression)};
}

star::Value star::AstPrinter::VisitLiteralExpr(std::shared_ptr<Literal> expr)
{
    return expr->m_Value;
}

star::Value star::AstPrinter::VisitUnaryExpr(std::shared_ptr<Unary> expr)
{
  return Value{TokenType::STRING, 
    Parenthesize(expr->m_Operator.GetLexeme(), expr->m_Right)};
}

star::Value star::AstPrinter::VisitTemplateLiteralExpr(std::shared_ptr<TemplateLiteral> expr)
{
    return Value{TokenType::STRING, 
        Parenthesize("template string", expr)};
}

star::Value star::AstPrinter::VisitTernaryExpr(std::shared_ptr<Ternary> expr)
{
    return Value{TokenType::STRING, 
        Parenthesize("ternary", expr)};
}

template<class... E>
std::string star::AstPrinter::Parenthesize(std::string_view name, E... expr)
{
  inner_assert((... && std::is_same_v<E, std::shared_ptr<Expr>>));
  std::ostringstream buffer;
  buffer << "(" << name;
  ((buffer << " " << Print(expr)), ...);
  buffer << ")";
  return buffer.str();
}

/*int main(){
  auto expression = std::make_shared<Binary>(
      std::make_shared<Unary>(
        Token(TokenType::MINUS, "-", nullptr, 1), 
        std::make_shared<Literal>(123)
      ),
      Token(TokenType::STAR, "*", nullptr, 1),
      std::make_shared<Grouping>(
       std::make_shared<Literal>(45.67)
      )
  );

  AstPrinter printer;
  std::cout << printer.print(expression) << '\n';
}*/
