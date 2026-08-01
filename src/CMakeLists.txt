
file(GLOB_RECURSE STAR_SRC_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS "star/*.cc")
file(GLOB_RECURSE STAR_HDR_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS "star/*.hh")

file(GLOB_RECURSE STAR_DEV_SRC_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS "star_dev/*.cc")
file(GLOB_RECURSE STAR_DEV_HDR_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS "star_dev/*.hh")

file(GLOB_RECURSE STAR_CLI_SRC_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS "star_cli/*.cc")
file(GLOB_RECURSE STAR_CLI_HDR_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS "star_cli/*.hh")

file(GLOB_RECURSE STAR_SRC_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS "star/*.cc")
file(GLOB_RECURSE STAR_HDR_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS "star/*.hh")
file(GLOB_RECURSE STAR_TESTS_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS "tests/*.cc")


find_package(magic_enum CONFIG REQUIRED)
find_package(pcre2 CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

set(LIB_TARGET_NAME star)
add_library(${LIB_TARGET_NAME} ${STAR_SRC_FILES} ${STAR_HDR_FILES})
set_cxx_project_standards(${LIB_TARGET_NAME} 20 FALSE)
target_include_directories(${LIB_TARGET_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/star>)
target_link_libraries(${LIB_TARGET_NAME} PRIVATE magic_enum::magic_enum
                                         PUBLIC PCRE2::8BIT PCRE2::POSIX)

set(LIB_CLI_TARGET_NAME star_cli)
add_library(${LIB_CLI_TARGET_NAME} ${STAR_CLI_SRC_FILES} ${STAR_CLI_HDR_FILES})
set_cxx_project_standards(${LIB_CLI_TARGET_NAME} 20 FALSE)
target_include_directories(${LIB_CLI_TARGET_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/star_cli>)
target_link_libraries(${LIB_CLI_TARGET_NAME} PRIVATE star)

set(LIB_DEV_TARGET_NAME star_dev)
add_library(${LIB_DEV_TARGET_NAME} ${STAR_DEV_SRC_FILES} ${STAR_DEV_HDR_FILES})
set_cxx_project_standards(${LIB_DEV_TARGET_NAME} 20 FALSE)
target_include_directories(${LIB_DEV_TARGET_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/star_dev>)
target_link_libraries(${LIB_DEV_TARGET_NAME} PRIVATE star)

if(WIN32)
    target_compile_definitions(${LIB_TARGET_NAME} PRIVATE STAR_PLATFORM_WINDOWS)
    target_compile_definitions(${LIB_CLI_TARGET_NAME} PRIVATE STAR_CLI_PLATFORM_WINDOWS)
    target_compile_definitions(${LIB_DEV_TARGET_NAME} PRIVATE STAR_DEV_PLATFORM_WINDOWS)
    if(BUILD_SHARED_LIBS)
        target_compile_definitions(${LIB_TARGET_NAME} PRIVATE STAR_EXPORTS INTERFACE STAR_IMPORTS)
        target_compile_definitions(${LIB_CLI_TARGET_NAME} PRIVATE STAR_CLI_EXPORTS INTERFACE STAR_CLI_IMPORTS)
        target_compile_definitions(${LIB_DEV_TARGET_NAME} PRIVATE STAR_DEV_EXPORTS INTERFACE STAR_DEV_IMPORTS)
    endif()
endif()

if(BUILD_CLI)
    set(EXECUTABLE_TARGET_NAME star_repl)
    add_executable(${EXECUTABLE_TARGET_NAME} repl/main.cc 
    repl/Repl.hh 
    repl/Repl.cc)
    target_link_libraries(${EXECUTABLE_TARGET_NAME} PRIVATE ${LIB_TARGET_NAME} 
                                                            ${LIB_CLI_TARGET_NAME}
                                                            ${LIB_DEV_TARGET_NAME})

    set_cxx_project_standards(${EXECUTABLE_TARGET_NAME} 20 FALSE)
    if(NOT WIN32)
        append_rpath(TARGET_NAME ${EXECUTABLE_TARGET_NAME})
    endif()
endif()

if(BUILD_TESTS)
    enable_testing()
    set(TESTS_TARGET_NAME star_tests)
    add_executable(${TESTS_TARGET_NAME} ${STAR_TESTS_FILES})
    target_link_libraries(${TESTS_TARGET_NAME} PRIVATE star GTest::gtest GTest::gtest_main)
    set_cxx_project_standards(${TESTS_TARGET_NAME} 20 FALSE)
    if(NOT WIN32)
        append_rpath(TARGET_NAME ${TESTS_TARGET_NAME})
    endif()
endif()
