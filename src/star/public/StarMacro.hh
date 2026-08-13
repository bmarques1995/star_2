#pragma once

#ifdef STAR_PLATFORM_WINDOWS
    #ifdef STAR_EXPORTS
        #define STAR_API __declspec(dllexport)
    #elif defined(STAR_IMPORTS)
        #define STAR_API __declspec(dllimport)
    #else
        #define STAR_API
    #endif
#else
    #define STAR_API
#endif