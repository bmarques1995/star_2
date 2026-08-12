#include "RuntimeError.hh"

star::RuntimeError::RuntimeError(const Token& token, const std::string& message) :
    token{token} 
{
    m_Reason = message;
}
