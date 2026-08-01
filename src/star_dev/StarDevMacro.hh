#pragma once

#ifdef STAR_DEV_PLATFORM_WINDOWS
    #ifdef STAR_DEV_EXPORTS
        #define STAR_DEV_API __declspec(dllexport)
    #elif defined(STAR_IMPORTS)
        #define STAR_DEV_API __declspec(dllimport)
    #else
        #define STAR_DEV_API
    #endif
#else
    #define STAR_DEV_API
#endif