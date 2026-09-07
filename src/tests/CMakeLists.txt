file(GLOB_RECURSE STAR_TESTS_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS "*.cc")

find_package(GTest CONFIG REQUIRED)

enable_testing()
set(TARGET_NAME star_tests)
add_executable(${TARGET_NAME} ${STAR_TESTS_FILES})
target_link_libraries(${TARGET_NAME} PRIVATE star GTest::gtest GTest::gtest_main)
set_cxx_project_standards(${TARGET_NAME} 20 FALSE)
set_artifacts_dir(${TARGET_NAME} ${PROJECT_BINARY_DIR})
set_msvc_target_utf8(${TARGET_NAME})
if(NOT WIN32)
    append_rpath(TARGET_NAME ${TARGET_NAME})
endif()
enable_coverage(${TARGET_NAME})

get_target_property(STAR_TESTS_RUNTIME_OUTPUT_DIRECTORY ${TARGET_NAME} RUNTIME_OUTPUT_DIRECTORY)

add_custom_target(copy_samples ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/samples
        ${STAR_TESTS_RUNTIME_OUTPUT_DIRECTORY}/samples
        COMMENT "Copying samples into binary directory")

add_dependencies(${TARGET_NAME} copy_samples)

target_installation_behaviour(TARGET_NAME ${TARGET_NAME}
                              CONFIG_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.cmake.in
                              VERSION ${PROJECT_VERSION}
                              PROJECT_NAME ${PROJECT_NAME}
                              NAMESPACE star::
                              COMPONENT star)
deploy_runtime_dependencies(${TARGET_NAME})
install_runtime_dependencies_for_libs(TARGETS GTest::gtest GTest::gtest_main)
