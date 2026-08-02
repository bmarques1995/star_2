cmake_minimum_required(VERSION 3.28)

project(Starlang VERSION 0.1.0)

option(BUILD_CLI "Build the command line interface" ON)
option(BUILD_TESTS "Build tests" ON)

option(ENABLE_LLVM_COVERAGE "Enable LLVM source coverage" OFF)

macro(enable_coverage target)
    if(ENABLE_LLVM_COVERAGE)
        target_compile_options(${target} PRIVATE -fprofile-instr-generate -fcoverage-mapping)
        target_link_options(${target} PRIVATE -fprofile-instr-generate)
    endif()
endmacro()

if(VCPKG_PREFIX_PATH)
    list(APPEND CMAKE_PREFIX_PATH "${VCPKG_PREFIX_PATH}")
endif()

include(FetchContent)

# 2. Declare the download location and version details
FetchContent_Declare(
    cmake_helpers
    GIT_REPOSITORY https://github.com/bmarques1995/cmake_helpers.git
    #GIT_TAG        v1.14.0 # Best practice: Use a specific tag or commit hash
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/cmake_helpers
)

# 3. Download the library and make its targets available
FetchContent_MakeAvailable(cmake_helpers)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake_helpers")

include(target_destination)

add_subdirectory(src)