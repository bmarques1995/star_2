set(TARGET_NAME star_repl)
set(REPL_SOURCES main.cc Repl.hh Repl.cc)

if(MSVC)
    set(ICON_PATH "${PROJECT_BINARY_DIR}/favicon.ico")
    
    # This generates a real app.rc in your build directory with the correct absolute path
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/app.rc.in"
        "${CMAKE_CURRENT_BINARY_DIR}/app.rc"
        @ONLY
    )
    
    # Add the generated .rc file to your source list
    set(REPL_SOURCES ${REPL_SOURCES} "${CMAKE_CURRENT_BINARY_DIR}/app.rc")
endif()

add_executable(${TARGET_NAME} ${REPL_SOURCES})
target_link_libraries(${TARGET_NAME} PRIVATE star star_cli)

set_cxx_project_standards(${TARGET_NAME} 20 FALSE)
set_artifacts_dir(${TARGET_NAME} ${PROJECT_BINARY_DIR})
set_msvc_target_utf8(${TARGET_NAME})
if(NOT WIN32)
    append_rpath(TARGET_NAME ${TARGET_NAME} BUILD_EXTRA_PATH ${ARTIFACT_PREFIX} INSTALL_EXTRA_PATH ${ARTIFACT_PREFIX})
endif()

target_installation_behaviour(TARGET_NAME ${TARGET_NAME}
                              CONFIG_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.cmake.in
                              VERSION ${PROJECT_VERSION}
                              PROJECT_NAME ${PROJECT_NAME}
                              NAMESPACE star::
                              COMPONENT star)
deploy_runtime_dependencies(${TARGET_NAME})
set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME "star")

