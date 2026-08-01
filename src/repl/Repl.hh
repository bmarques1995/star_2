#pragma once

#include <string>

namespace star {

    class Star {
    public:
        static void RunFile(const std::string& filePath);
        static void RunPrompt();
    private:
        static void Run(const std::string& source);

    };

}