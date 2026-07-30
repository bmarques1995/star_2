
file(GLOB_RECURSE STAR_SRC_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS "star/*.cc")
file(GLOB_RECURSE STAR_HDR_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS "star/*.hh")

find_package(magic_enum CONFIG REQUIRED)

set(LIB_TARGET_NAME star)
add_library(${LIB_TARGET_NAME} ${STAR_SRC_FILES} ${STAR_HDR_FILES})
set_cxx_project_standards(${LIB_TARGET_NAME} 20 FALSE)
target_include_directories(${LIB_TARGET_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/star>)
target_link_libraries(${LIB_TARGET_NAME} PRIVATE magic_enum::magic_enum)

if(WIN32)
    target_compile_definitions(${LIB_TARGET_NAME} PRIVATE STAR_PLATFORM_WINDOWS)
    if(BUILD_SHARED_LIBS)
        target_compile_definitions(${LIB_TARGET_NAME} PRIVATE STAR_EXPORTS INTERFACE STAR_IMPORTS)
    endif()
endif()

if(BUILD_CLI)
    set(EXECUTABLE_TARGET_NAME star_cli)
    add_executable(${EXECUTABLE_TARGET_NAME} repl/main.cc)
    target_link_libraries(${EXECUTABLE_TARGET_NAME} ${LIB_TARGET_NAME})

    set_cxx_project_standards(${EXECUTABLE_TARGET_NAME} 20 FALSE)
    append_rpath(TARGET_NAME ${EXECUTABLE_TARGET_NAME})
endif()