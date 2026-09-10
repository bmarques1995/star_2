#include "CLInterpreter.hh"
#include "Print.hh"

star::CLInterpreter::CLInterpreter()
{
	m_CurrentEnv->Define(Token{ TokenType::FUN, "print", 1, 1, "::native" }, Value{ std::make_shared<Print>() });
}
