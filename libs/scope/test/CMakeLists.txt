
cmake_minimum_required(VERSION 3.0.0)

set(TEST_PROJECT_NAME
    scope-test
)

file(GLOB TEST_FILES
    *.cpp
)

set(LINKING_LIBRARIES
  
)

set(LIBRARY_TESTS_SOURCE
    ${TEST_FILES}
)

project(${TEST_PROJECT_NAME})

enable_testing()

if (NOT (TARGET gtest AND TARGET gtest_main))
    message(FATAL_ERROR "gtest and gtest_main targets NOT found")
endif()

add_executable(${TEST_PROJECT_NAME} ${LIBRARY_TESTS_SOURCE})
target_link_libraries(${TEST_PROJECT_NAME}
    gtest
    gtest_main
    ${LIBRARY_NAME}
    ${LINKING_LIBRARIES} # NOTE: This is defined from project above
)
add_test(${TEST_PROJECT_NAME} ${TEST_PROJECT_NAME})
