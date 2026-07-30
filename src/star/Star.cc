#include "Star.hh"
#include "ColorToken.hh"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "Scanner.hh"
#include "Debug.hh"

namespace fs = std::filesystem;

void star::Star::RunFile(const std::string& filePath)
{
    if(!fs::exists(filePath))
    {
        std::cerr << ColorToken::s_Red <<"File not found: " << filePath << ColorToken::s_EndColor << "\n";
        std::exit(66);
    }

    std::ifstream file(filePath, std::ios::binary | std::ios::ate);

    if(!file)
    {
        std::cerr << ColorToken::s_Red << "Permission denied to open file: " << filePath << ColorToken::s_EndColor <<"\n";
        std::exit(77);
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> buffer(size);

    if(!file.read(buffer.data(), size))
    {
        std::cerr << ColorToken::s_Red << "Error reading file." << ColorToken::s_EndColor << "\n";
    }

    std::string content(buffer.begin(), buffer.end());
    Run(content);
    if(Debug::HadError()){ std::exit(65); }
}

void star::Star::RunPrompt()
{
    std::string line;
    static const char* replPrefix = "star> ";
    std::cout << replPrefix;
    for(;;)
    {
        if(!std::getline(std::cin, line) || (line == "exit"))
            break;
        Run(line);
        if(Debug::HadError()){ std::exit(65); }
        std::cout << "\n";
        std::cout << replPrefix;
    }
}

void star::Star::Run(const std::string& source)
{
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.ScanTokens();
    for(auto& t: tokens)
    {
        std::cout << ColorToken::s_Green << t << ColorToken::s_EndColor << "\n";
    }
}