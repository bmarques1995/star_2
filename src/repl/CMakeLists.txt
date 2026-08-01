set(TARGET_NAME star_repl)
add_executable(${TARGET_NAME} main.cc Repl.hh Repl.cc)
target_link_libraries(${TARGET_NAME} PRIVATE star star_cli star_dev)

set_cxx_project_standards(${TARGET_NAME} 20 FALSE)
if(NOT WIN32)
    append_rpath(TARGET_NAME ${TARGET_NAME})
endif()
