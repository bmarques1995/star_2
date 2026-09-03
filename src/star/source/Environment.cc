#include "Environment.hh"
#include "RuntimeError.hh"
star::Environment::Environment() {}

void star::Environment::Define(const Token& name, Value value)
{
	auto it = m_Values.find(name.GetLexeme());
	if (it != m_Values.end())
	{
		throw RuntimeError(name, "Variable already defined: '" + name.GetLexeme() + "'.");
	}
	m_Values.insert_or_assign(name.GetLexeme(), std::move(value));
}

void star::Environment::Reassign(const Token& name, Value value)
{
	auto it = m_Values.find(name.GetLexeme());
	if (it == m_Values.end())
	{
		throw RuntimeError(name, "Cannot reassign undefined variable: '" + name.GetLexeme() + "'.");
	}
	if(it->second.GetType() != VariableType::Dynamic && it->second.GetType() != value.GetType())
	{
		throw RuntimeError(name, "Cannot reassign variable with different type: '" + name.GetLexeme() + "'.");
	}
	m_Values.insert_or_assign(name.GetLexeme(), std::move(value));
}

star::Value star::Environment::Get(const Token& name) 
{
	auto elem = m_Values.find(name.GetLexeme());
	if (elem != m_Values.end()) {
		return elem->second;
	}
	throw RuntimeError(name, "Undefined variable: '" + name.GetLexeme() + "'.");
}
