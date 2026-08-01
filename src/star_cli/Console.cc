#include "Console.hh"
#include "ColorToken.hh"
#include <iostream>

star::TraceConsole& star::TraceConsole::operator<<(const std::string& value)
{
    std::cout << star::ColorToken::s_Green << value << star::ColorToken::s_EndColor << "\n";
    return *this;
}

star::WarnConsole& star::WarnConsole::operator<<(const std::string& value)
{
    std::cout << star::ColorToken::s_Yellow << value << star::ColorToken::s_EndColor << "\n";
    return *this;
}

star::ErrorConsole& star::ErrorConsole::operator<<(const std::string& value)
{
    std::cout << star::ColorToken::s_Red << value << star::ColorToken::s_EndColor << "\n";
    return *this;
}
        