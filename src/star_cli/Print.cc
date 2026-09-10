#include "Print.hh"
#include "Console.hh"

star::Print::Print() 
	: m_Arity(1)
{
}

const size_t star::Print::Arity() const
{
	return m_Arity;
}

std::string star::Print::ToString() const
{
	return "<print>";
}

star::Value star::Print::Call(Interpreter& interpreter, std::vector<Value> args)
{
	NeutralConsole() << args[0].ToString() << "\n";
	return {TokenType::VOID, ""};
}
