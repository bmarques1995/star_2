#include "Environment.hh"
#include "RuntimeError.hh"
star::Environment::Environment() :
	m_Parent(nullptr)
{
}

star::Environment::Environment(std::shared_ptr<Environment> parent) :
	m_Parent(parent)
{
}

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
		if (m_Parent != nullptr)
			m_Parent->Reassign(name, value);
		else
			throw RuntimeError(name, "Cannot reassign undefined variable: '" + name.GetLexeme() + "'.");
	}
	if(it->second.GetType() != VariableType::Dynamic && it->second.GetType() != value.GetAssignedType())
	{
		throw RuntimeError(name, "Cannot reassign variable with different type: '" + name.GetLexeme() + "'.");
	}
	if(it->second.GetType() != VariableType::Dynamic && value.GetType() == VariableType::Dynamic)
	{
		value.LockType();
	}
	m_Values.insert_or_assign(name.GetLexeme(), std::move(value));
}

star::Value star::Environment::Get(const Token& name) 
{
	auto elem = m_Values.find(name.GetLexeme());
	if (elem != m_Values.end()) {
		return elem->second;
	}
	if (m_Parent != nullptr) {
		return m_Parent->Get(name);
	}
	throw RuntimeError(name, "Undefined variable: '" + name.GetLexeme() + "'.");
}
