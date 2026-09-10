add_subdirectory(star)


if(BUILD_CLI)
    add_subdirectory(star_cli)
    add_subdirectory(repl)    
endif()

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
