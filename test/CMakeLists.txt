cmake_minimum_required(VERSION 3.0.0)

set(TEST_PROJECT_NAME
    scope-test
)

file( GLOB_RECURSE TEST_FILES *.cpp )

set(LIBRARY_TESTS_SOURCE
    ${TEST_FILES}
)

project(${TEST_PROJECT_NAME})

enable_testing()

if (NOT (TARGET gtest AND TARGET gtest_main))
    message(FATAL_ERROR "gtest and gtest_main targets NOT found")
endif()

include_directories(../include/Scope)
include_directories(../3party/gtest/googletest/include ../3party/gtest/googlemock/include)
include_directories(../3party/msgpackc/include)

add_executable(${TEST_PROJECT_NAME} ${LIBRARY_TESTS_SOURCE})
target_link_libraries(${TEST_PROJECT_NAME}
    gtest
    gtest_main
    Gemmi
)
add_test(${TEST_PROJECT_NAME} ${TEST_PROJECT_NAME})
