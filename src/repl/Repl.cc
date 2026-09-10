#include "Repl.hh"
#include "BaseException.hh"
#include <Console.hh>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "Scanner.hh"
#include "Parser.hh"
#include "Version.hh"
#include "EscapeObject.hh"


namespace fs = std::filesystem;
star::CLInterpreter star::Star::s_Interpreter;

void star::Star::RunFile(const std::string& filePath)
{
    if(!fs::exists(filePath))
    {
        star::ErrorConsole() << "File not found: " << filePath;
        std::exit(66);
    }

    std::ifstream file(filePath, std::ios::binary | std::ios::ate);

    if(!file)
    {
        star::ErrorConsole() << "Permission denied to open file: " << filePath;
        std::exit(77);
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> buffer(size);

    if(!file.read(buffer.data(), size))
    {
        star::ErrorConsole() << "Error reading file.";
    }

    std::string content(buffer.begin(), buffer.end());
    Run(content, filePath);
}

void star::Star::RunPrompt()
{
    std::string line;
    static const char* replPrefix = "star> ";
    star::NeutralConsole() << "[star version: " << Version::GetVersion() << "]" << "\n";
    star::NeutralConsole() << "[" << Version::GetCompilerNameAndVersion() << "]" << "\n\n";
    star::TraceConsole() << replPrefix;
    try
    {
        for (;;)
        {
            if (!std::getline(std::cin, line))
                break;
            Run(line);
            star::TraceConsole() << replPrefix;
        }
    }
    catch(const EscapeObject& e)
    {
        star::TraceConsole() << "Core terminated with code: " << std::to_string(e.m_EscapeCode) << "\n";
		std::exit(e.m_EscapeCode);
    }
    
}

void star::Star::Run(const std::string& source, const std::string& filepath)
{
    try
    {
        Scanner scanner(source, filepath);
        std::vector<Token> tokens = scanner.ScanTokens();
        Parser parser{tokens};
        std::vector<std::shared_ptr<Statement::Stmt>> statements = parser.Parse();
        s_Interpreter.Interpret(statements);
    }
    catch(ScriptException e)
    {
        star::ErrorConsole() << e.what();
        std::exit(65);
    }
}
