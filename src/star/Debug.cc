#include "Debug.hh"
#include <iostream>
#include "ColorToken.hh"

void star::Debug::Report(uint32_t line, const std::string& where, const std::string& message)
{
    s_HadError = true;
    std::cerr << ColorToken::s_Red << "Error at line: " << line << ", on: " << where << "\n" << message;
}

void star::Debug::Error(uint32_t line, const std::string& message)
{
    Report(line, "", message);
}

bool star::Debug::HadError()
{
    return s_HadError;
}