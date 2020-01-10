
set(SE_SCOPE_TESTS_NAME ${SE_SCOPE_PROJECT_NAME}_TEST)

project(${SE_SCOPE_TESTS_NAME} C CXX)

include(cmake/UnitTestFiles.cmake)

if(NOT TARGET gtest)
    add_subdirectory(cmake/gtest)
endif()

add_test(${SE_SCOPE_TESTS_NAME} ${SE_SCOPE_TESTS_NAME})

add_executable(${SE_SCOPE_TESTS_NAME}
        ${SE_SCOPE_UNIT_TESTS}
        ${SE_SCOPE_TEST_MOCKS}
        )

target_include_directories(${SE_SCOPE_TESTS_NAME} PUBLIC cmake-build-debug/gtest/googletest-src/googletest/include)
target_include_directories(${SE_SCOPE_TESTS_NAME} PUBLIC cmake-build-debug/gtest/googletest-src/googlemock/include)
target_include_directories(${SE_SCOPE_TESTS_NAME} PRIVATE ../include/Scope)
target_include_directories(${SE_SCOPE_TESTS_NAME} PRIVATE ../public)
target_include_directories(${SE_SCOPE_TESTS_NAME} PRIVATE ../ext/se-lib-c/include)
target_include_directories(${SE_SCOPE_TESTS_NAME} PRIVATE .)

target_link_libraries(
        ${SE_SCOPE_TESTS_NAME}
        PUBLIC
        $<BUILD_INTERFACE:se-lib-c>
        $<BUILD_INTERFACE:protobuf-nanopb-static>
        $<BUILD_INTERFACE:se-scope>
        $<BUILD_INTERFACE:gtest_main>)