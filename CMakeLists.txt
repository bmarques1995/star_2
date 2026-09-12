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

if(VCPKG_PREFIX_PATH AND VCPKG_TARGET_TRIPLET)
    set(CMAKE_PREFIX_PATH "${VCPKG_PREFIX_PATH}")
endif()

include(FetchContent)

set(ARTIFACT_PREFIX ${PROJECT_BINARY_DIR})
if(WIN32)
    set(ARTIFACT_PREFIX ${CMAKE_INSTALL_PREFIX})
endif()

if(USES_VCPKG)
    set(VCPKG_CURRENT_PREFIX ${VCPKG_PREFIX_PATH}/${VCPKG_TARGET_TRIPLET})
    list(APPEND CMAKE_PREFIX_PATH ${VCPKG_CURRENT_PREFIX})
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		set(VCPKG_CURRENT_PREFIX ${VCPKG_CURRENT_PREFIX}/debug)
    endif()
    if(WIN32)
        set(ARTIFACT_PREFIX ${VCPKG_CURRENT_PREFIX})
    endif()
endif()

# 2. Declare the download location and version details
FetchContent_Declare(
    cmake_helpers
    GIT_REPOSITORY https://github.com/bmarques1995/cmake_helpers.git
    #GIT_TAG        v1.14.0 # Best practice: Use a specific tag or commit hash
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/cmake_helpers
)

# 3. Download the library and make its targets available
FetchContent_MakeAvailable(cmake_helpers)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR};${PROJECT_SOURCE_DIR}/cmake_helpers")

include(target_destination)

include(InstallRequiredSystemLibraries)
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(cpack_patcher)
set_cpack_defaults(LICENSE_IN_FILE ${PROJECT_SOURCE_DIR}/LICENSE LICENSE_OUT_FILE ${PROJECT_BINARY_DIR}/LICENSE.txt)
add_cpack_wix_configs(
    UPGRADE_GUID "C411F940-3D69-482A-A5EC-229F8A734F14"
    UI "StarlangUI"
    PATCH "${PROJECT_SOURCE_DIR}/wix/wix_patch.xml"
    EXTENSIONS "WixToolset.UI.wixext" "WixToolset.Util.wixext"
    EXTRA_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/wix/StarlangUI.wxs"
    EXTRA_ACTIONS "STARLANG_ADD_TO_PATH"
)
add_cpack_unix_packers(
    MAINTAINER "bmarques1995"
    URL "https://github.com/bmarques1995/star_2.git"
    LICENSE BSD-3-Clauses
    DESC "<DESC>"
    COMMENT "<COMMENT>"
    LICENSE_FILE "${PROJECT_SOURCE_DIR}/LICENSE")
include(CPack)

add_subdirectory(src)