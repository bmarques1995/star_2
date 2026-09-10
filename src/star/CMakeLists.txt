file(GLOB_RECURSE STAR_SRC_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS "source/*.cc")
file(GLOB_RECURSE STAR_PVT_HDR_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS "public/*.hh")
file(GLOB_RECURSE STAR_PBL_HDR_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS "private/*.hh")

find_package(magic_enum CONFIG REQUIRED)
find_package(pcre2 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

set(TARGET_NAME star)
add_library(${TARGET_NAME} ${STAR_SRC_FILES} ${STAR_PBL_HDR_FILES} ${STAR_PVT_HDR_FILES})
set_cxx_project_standards(${TARGET_NAME} 20 FALSE)
set_artifacts_dir(${TARGET_NAME} ${PROJECT_BINARY_DIR})
set_msvc_target_utf8(${TARGET_NAME})
target_include_directories(${TARGET_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
                                                 $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public/builtin>
                                                 $<INSTALL_INTERFACE:include>
                                          PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/private>)
target_link_libraries(${TARGET_NAME} PRIVATE magic_enum::magic_enum
                                         PUBLIC PCRE2::8BIT PCRE2::POSIX fmt::fmt)
enable_coverage(${TARGET_NAME})
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${TARGET_NAME} PUBLIC STAR_DEBUG)
endif()

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    target_compile_definitions(${TARGET_NAME} PUBLIC STAR_PLATFORM_WINDOWS WIN32_LEAN_AND_MEAN NOMINMAX)
    if(BUILD_SHARED_LIBS)
        target_compile_definitions(${TARGET_NAME} PRIVATE STAR_EXPORTS INTERFACE STAR_IMPORTS)
    endif()
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
	target_compile_definitions(${TARGET_NAME} PUBLIC STAR_PLATFORM_MACOS)
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
	target_compile_definitions(${TARGET_NAME} PUBLIC STAR_PLATFORM_LINUX)
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "FreeBSD")
    target_compile_definitions(${TARGET_NAME} PUBLIC STAR_PLATFORM_FREEBSD)
endif()

target_installation_behaviour(TARGET_NAME ${TARGET_NAME}
                              CONFIG_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.cmake.in
                              VERSION ${PROJECT_VERSION}
                              PROJECT_NAME ${PROJECT_NAME}
                              NAMESPACE star::
                              COMPONENT star)

deploy_runtime_dependencies(${TARGET_NAME})
install_runtime_dependencies_for_libs(TARGETS magic_enum::magic_enum PCRE2::8BIT PCRE2::POSIX fmt::fmt)
