#include "Exit.hh"
#include "RuntimeError.hh"

star::Exit::Exit() :
	m_Arity(1),
	m_ExpectedTypes({ VariableType::Integer32 })
{
}

const size_t star::Exit::Arity() const
{
	return m_Arity;
}

std::string star::Exit::ToString() const
{
	return "<exit: void>";
}

star::Value star::Exit::Call(Interpreter& interpreter, std::vector<Value> args)
{
	Token token{ TokenType::FUN, "<exit>", 1, 1, "::native" };
	if(args.size() != m_Arity)
		throw RuntimeError(token, "Invalid number of arguments for exit function");

	if(args[0].GetAssignedType() != VariableType::Integer32)
		throw RuntimeError(token, "Invalid argument type for exit function");

	int32_t exitCode = std::get<int32_t>(args[0].GetRValue());
	std::exit(exitCode);
	return { TokenType::VOID, "" };
}
