#pragma once

#ifdef STAR_CLI_PLATFORM_WINDOWS
    #ifdef STAR_CLI_EXPORTS
        #define STAR_CLI_API __declspec(dllexport)
    #elif defined(STAR_IMPORTS)
        #define STAR_CLI_API __declspec(dllimport)
    #else
        #define STAR_CLI_API
    #endif
#else
    #define STAR_CLI_API
#endif