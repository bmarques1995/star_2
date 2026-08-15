#include "CLInterpreter.hh"
#include "Console.hh"

star::CLInterpreter::CLInterpreter()
{
}

void star::CLInterpreter::Write(const std::string& text) const
{
    TraceConsole() << text << "\n";
}
