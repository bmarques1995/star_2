file(GLOB_RECURSE STAR_SRC_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS "*.cc")
file(GLOB_RECURSE STAR_HDR_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS "*.hh")

find_package(magic_enum CONFIG REQUIRED)
find_package(pcre2 CONFIG REQUIRED)

set(TARGET_NAME star)
add_library(${TARGET_NAME} ${STAR_SRC_FILES} ${STAR_HDR_FILES})
set_cxx_project_standards(${TARGET_NAME} 20 FALSE)
target_include_directories(${TARGET_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(${TARGET_NAME} PRIVATE magic_enum::magic_enum
                                         PUBLIC PCRE2::8BIT PCRE2::POSIX)

enable_coverage(${TARGET_NAME})
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${TARGET_NAME} PUBLIC STAR_DEBUG)
endif()

if(WIN32)
    target_compile_definitions(${TARGET_NAME} PRIVATE STAR_PLATFORM_WINDOWS)
    if(BUILD_SHARED_LIBS)
        target_compile_definitions(${TARGET_NAME} PRIVATE STAR_EXPORTS INTERFACE STAR_IMPORTS)
    endif()
endif()
