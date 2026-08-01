#include "Debug.hh"
#include <sstream>

void star::Debug::Report(size_t line, const std::string& where, const std::string& message)
{
    s_HadError = true;
    std::stringstream errorBuffer;
    errorBuffer << "Error at line: " << line << ", on: " << where << "\n" << message;
    s_Report = errorBuffer.str();
}

void star::Debug::Error(size_t line, const std::string& message)
{
    Report(line, "", message);
}

bool star::Debug::HadError()
{
    return s_HadError;
}

const std::string& star::Debug::GetLastReport()
{
    return s_Report;
}