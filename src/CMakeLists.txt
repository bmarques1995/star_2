add_subdirectory(star)
add_subdirectory(star_cli)

if(BUILD_CLI)
    add_subdirectory(repl)    
endif()

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
