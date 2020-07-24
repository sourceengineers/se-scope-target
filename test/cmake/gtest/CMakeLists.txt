cmake_minimum_required (VERSION 3.5)

project (GoogleTest)

# googletest: download and build at configuration time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
		RESULT_VARIABLE result
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
	message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
		RESULT_VARIABLE result
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
	message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
		${CMAKE_CURRENT_BINARY_DIR}/googletest-build
		EXCLUDE_FROM_ALL)

add_library(gtest-lib INTERFACE)
target_include_directories(gtest-lib INTERFACE
		${gmock_SOURCE_DIR}
		${gmock_SOURCE_DIR}/include
		${gtest_SOURCE_DIR}
		${gtest_SOURCE_DIR}/include)
target_link_libraries(gtest-lib INTERFACE gtest gtest_main pthread)
add_library(test::gtest ALIAS gtest-lib)

target_link_libraries(gtest-lib INTERFACE test::gtest)
