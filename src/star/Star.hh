#pragma once

#include "StarMacro.hh"
#include <string>

namespace star {

    class STAR_API Star {
    public:
        static void RunFile(const std::string& filePath);
        static void RunPrompt();
    private:
        static void Run(const std::string& source);

    };

}