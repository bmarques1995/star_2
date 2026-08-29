#include <iostream>
#include <cstdlib>
#include "Repl.hh"

#ifdef STAR_PLATFORM_WINDOWS

#include <windows.h>

#endif // 


int main(int argc, char** argv)
{
#ifdef STAR_PLATFORM_WINDOWS
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif
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

