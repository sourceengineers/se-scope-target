cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 11)

# Set the Project name
set(PROJECT_NAME se-scope)
################################################################################
# Set options
################################################################################
# Inform the compiler to compile for 32 bit code
add_definitions(-DARCH_SIZE_32=1)

project(${PROJECT_NAME} C ASM)

################################################################################
# Set options
################################################################################
set(PROJECT_SOURCE_DIR src)
set(PROJECT_DEPENDENCIES_DIR 3rdparty)
set(PROJECT_DEPENDENCIES jsmn)
set(PROJECT_INCLUDE_DIR include/Scope)
set(PROJECT_INCLUDE_DIR_PUBLIC public)
set(PROJECT_TEST_DIR test)

#######################################################################################################################
## Find all needed files
#######################################################################################################################
file(GLOB_RECURSE PROJECT_SOURCE "${PROJECT_SOURCE_DIR}/*.c")
file(GLOB_RECURSE PROJECT_HEADER "${PROJECT_INCLUDE_DIR}/*.h")
file(GLOB_RECURSE PROJECT_HEADER_PUBLIC "${PROJECT_INCLUDE_DIR_PUBLIC}/*.h")
list (APPEND PROJECT_HEADER ${PROJECT_HEADER_PUBLIC})

################################################################################
# GTest
################################################################################
enable_testing()
option(BUILD_GMOCK ON)
add_subdirectory(${PROJECT_DEPENDENCIES_DIR}/gtest)
set(gtest_SOURCE_DIR ${PROJECT_DEPENDENCIES_DIR}/gtest/googletest)
add_subdirectory(test)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

################################################################################
# Include dirs and link libraries
################################################################################
include_directories("${PROJECT_DEPENDENCIES_DIR}/jsmn" "${PROJECT_DEPENDENCIES_DIR}/nanopb" "${PROJECT_INCLUDE_DIR}" "${PROJECT_INCLUDE_DIR_PUBLIC}")

add_subdirectory(${PROJECT_DEPENDENCIES_DIR}/jsmn)
add_subdirectory(${PROJECT_DEPENDENCIES_DIR}/nanopb)

add_library(${PROJECT_NAME} STATIC ${PROJECT_SOURCE})
target_link_libraries(${PROJECT_NAME} jsmn protobuf-nanopb-static)

################################################################################
# Install project
################################################################################
TARGET_INCLUDE_DIRECTORIES(se-scope
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        )

message("-- Project will be installed to: ${CMAKE_INSTALL_PREFIX}")
install(TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/" COMPONENT library)
install(DIRECTORY "${PROJECT_INCLUDE_DIR}/" DESTINATION "${CMAKE_INSTALL_PREFIX}/include")
install(DIRECTORY "${PROJECT_INCLUDE_DIR_PUBLIC}/" DESTINATION "${CMAKE_INSTALL_PREFIX}/include")
