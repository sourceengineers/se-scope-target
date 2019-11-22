cmake_minimum_required(VERSION 3.0.0)

set(TEST_PROJECT_NAME
        scope-test
        )

file(GLOB_RECURSE TEST_FILES *.cpp)

set(LIBRARY_TESTS_SOURCE
        ${TEST_FILES}
        )

project(${TEST_PROJECT_NAME})

enable_testing()

if (NOT (TARGET gtest AND TARGET gtest_main))
    message(FATAL_ERROR "gtest and gtest_main targets NOT found")
endif ()

include_directories(../include/Scope ../public)
include_directories(../3rdparty/gtest/googletest/include ../3rdparty/gtest/googlemock/include)

add_executable(${TEST_PROJECT_NAME} ${LIBRARY_TESTS_SOURCE})
target_link_libraries(${TEST_PROJECT_NAME}
        gtest
        gtest_main
        se-scope
        )

add_test(${TEST_PROJECT_NAME} ${TEST_PROJECT_NAME})
