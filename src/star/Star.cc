#include "Star.hh"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>

namespace fs = std::filesystem;
static const char* errorCliColorBeg = "\033[31m";
static const char* errorCliColorEnd = "\033[0m";

void star::Star::RunFile(const std::string& filePath)
{
    if(!fs::exists(filePath))
    {
        std::cerr << errorCliColorBeg <<"File not found: " << filePath << errorCliColorEnd << "\n";
        std::exit(66);
    }

    std::ifstream file(filePath, std::ios::binary | std::ios::ate);

    if(!file)
    {
        std::cerr << errorCliColorBeg << "Permission denied to open file: " << filePath << errorCliColorEnd <<"\n";
        std::exit(77);
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> buffer(size);

    if(!file.read(buffer.data(), size))
    {
        std::cerr << errorCliColorBeg << "Error reading file." << errorCliColorEnd << "\n";
    }

    std::string content(buffer.begin(), buffer.end());
    Run(content);
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
        std::cout << "\n";
        std::cout << replPrefix;
    }
}

void star::Star::Run(const std::string& source)
{
    std::cout << source;
}