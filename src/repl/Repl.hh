#pragma once

#include <string>
#include "CLInterpreter.hh"

namespace star {

    class Star {
    public:
        static void RunFile(const std::string& filePath);
        static void RunPrompt();
    private:
        static void Run(const std::string& source, const std::string& filepath = "::repl");
        static CLInterpreter s_Interpreter;
    };

}