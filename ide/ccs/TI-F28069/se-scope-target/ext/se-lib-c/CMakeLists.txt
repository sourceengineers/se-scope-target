cmake_minimum_required (VERSION 3.5)

# Let the user choose if he wants to build the tests
option(SE_LIB_C_BUILD_TESTS "Build the tests" ON)

# Set the name of the library
set(SE_LIB_C_PROJECT_NAME se-lib-c)

project (${SE_LIB_C_PROJECT_NAME})

set(SE_LIB_C_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

#Add the library sources
include(cmake/SeLibCSources.cmake)
add_library(${SE_LIB_C_PROJECT_NAME} STATIC ${SE_LIB_C_SOURCES})
target_include_directories(
		${SE_LIB_C_PROJECT_NAME}
		PUBLIC
		include
)

# If wanted, build the tests
if(SE_LIB_C_BUILD_TESTS)
	enable_testing()
	set(SE_LIB_C_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test)
	add_subdirectory(test)
endif()
