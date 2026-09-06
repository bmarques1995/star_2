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
    star::NeutralConsole() << "[star version: " << "in development" << "]" << "\n";
    star::NeutralConsole() << "[" << GetCompilerNameAndVersion() << "]" << "\n\n";
    star::TraceConsole() << replPrefix;
    for(;;)
    {
        if(!std::getline(std::cin, line) || (line == "exit"))
            break;
        Run(line);
        star::TraceConsole() << replPrefix;
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

std::string star::Star::GetCompilerNameAndVersion()
{
    std::stringstream ss;
#if defined(_MSC_VER)
    uint32_t major = _MSC_FULL_VER / 10'000'000;
    uint32_t minor = (_MSC_FULL_VER % 10'000'000) / 100'000;
    uint32_t build = (_MSC_FULL_VER % 100'000);
    ss << "Compiler: MSVC (cl) " << major << "." << minor << "." << build;
#elif defined(__clang__)
    ss << "Compiler: clang++ " << __clang_major__ << "." << __clang_minor__ << "." << __clang_patchlevel__;
#elif defined(__GNUC__)
    ss << "Compiler: g++ "
        << __GNUC__ << "."
        << __GNUC_MINOR__ << "."
        << __GNUC_PATCHLEVEL__;
#else
    ss << "Compiler: Unknown / Generic Compiler\n";
#endif
    std::string compilerName = ss.str();
    return compilerName;
}
