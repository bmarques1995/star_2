file(GLOB_RECURSE STAR_TESTS_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS "*.cc")

find_package(GTest CONFIG REQUIRED)

enable_testing()
set(TARGET_NAME star_tests)
add_executable(${TARGET_NAME} ${STAR_TESTS_FILES})
target_link_libraries(${TARGET_NAME} PRIVATE star GTest::gtest GTest::gtest_main)
set_cxx_project_standards(${TARGET_NAME} 20 FALSE)
if(NOT WIN32)
    append_rpath(TARGET_NAME ${TARGET_NAME})
endif()

add_custom_target(copy_samples ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/samples
        ${CMAKE_CURRENT_BINARY_DIR}/samples
        COMMENT "Copying samples into binary directory")

add_dependencies(${TARGET_NAME} copy_samples)
