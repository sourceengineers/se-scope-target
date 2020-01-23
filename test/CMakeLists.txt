
set(SE_SCOPE_TESTS ${SE_SCOPE}-test)

project(${SE_SCOPE_TESTS} C CXX)

include(cmake/UnitTestFiles.cmake)

if(NOT TARGET gtest)
    add_subdirectory(cmake/gtest)
endif()

add_test(${SE_SCOPE_TESTS} ${SE_SCOPE_TESTS})

add_executable(${SE_SCOPE_TESTS}
        ${SE_SCOPE_UNIT_TESTS}
        ${SE_SCOPE_TEST_MOCKS}
        )

target_include_directories(${SE_SCOPE_TESTS} PRIVATE ${gmock_SOURCE_DIR}/include)
target_include_directories(${SE_SCOPE_TESTS} PRIVATE ${gmock_SOURCE_DIR})
target_include_directories(${SE_SCOPE_TESTS} PRIVATE ${gtest_SOURCE_DIR}/include)
target_include_directories(${SE_SCOPE_TESTS} PRIVATE ${gtest_SOURCE_DIR})
target_include_directories(${SE_SCOPE_TESTS} PRIVATE ../include/Scope)
target_include_directories(${SE_SCOPE_TESTS} PRIVATE ../public)
target_include_directories(${SE_SCOPE_TESTS} PRIVATE ../ext/se-lib-c/include)
target_include_directories(${SE_SCOPE_TESTS} PRIVATE .)

target_link_libraries(
        ${SE_SCOPE_TESTS}
        PRIVATE
        protobuf-nanopb-static
        ${SE_SCOPE}
        gtest
        gtest_main
        pthread)