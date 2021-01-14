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

target_include_directories(${SE_SCOPE_TESTS} PRIVATE . ${SE_SCOPE_PROJECT_INCLUDE_DIR})

target_compile_options(${SE_SCOPE} PUBLIC -DUNIT_TEST)

target_link_libraries(
        ${SE_SCOPE_TESTS}
        PRIVATE
        se-lib::scope
        test::gtest
)