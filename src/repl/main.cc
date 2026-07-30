#include <iostream>
#include <cstdlib>
#include "Star.hh"

int main(int argc, char** argv)
{
    if(argc > 2)
    {
        std::cerr << "Usage: " << argv[0] << " file.st" << std::endl;
        return EXIT_FAILURE;
    }

    if(argc == 2)
    {
        star::Star::RunFile(argv[1]);
    }
    else
    {
        star::Star::RunPrompt();
    }
    
    return EXIT_SUCCESS;
}

