#include "RuntimeError.hh"
#include <sstream>

star::RuntimeError::RuntimeError(const Token& token, const std::string& message) :
   m_Token{token}
{
   std::stringstream ss;
   ss << "ParserException: " << message << " at: " << m_Token;
   m_Reason = ss.str();
}
