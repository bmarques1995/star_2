#include "BaseException.hh"

star::ScriptException::ScriptException()
    : m_Reason("")
{
}

const char* star::ScriptException::what() const noexcept
{
    return m_Reason.c_str();
}
