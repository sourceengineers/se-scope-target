cmake_minimum_required(VERSION 3.0.0)

set (CMAKE_CXX_STANDARD 11)

set(ARCHITECTURE_32BIT ON)

set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

if(ARCHITECTURE_32BIT)
    set(CMAKE_C_FLAGS -m32)
    set(CMAKE_CXX_FLAGS -m32)
    set(MSGPACK_32BIT ON)
endif()

# How to build:
# mkdir build 
# cd build
# Pick the right configuration:
# Debug: cmake -DCMAKE_BUILD_TYPE=Debug ..
# Release: cmake -DCMAKE_BUILD_TYPE=Release ..
# make

# Set the Project name
set(PROJECT_NAME
    Gemmi
)
set(PROJECT_SOURCE_DIR
    src
)
set(PROJECT_DEPENDENCIES_DIR
    3party
)
set(PROJECT_DEPENDENCIES
  msgpackc
)
set(PROJECT_LIBRARIES_DIR
    include/Scope
)
set(PROJECT_TEST_DIR
    test
)

project(${PROJECT_NAME})
#######################################################################################################################
## Find all needed files
#######################################################################################################################
file( GLOB_RECURSE PROJECT_SOURCE "${PROJECT_SOURCE_DIR}/*.c" )

################################################################################
# GTest 
################################################################################
enable_testing()
set(BUILD_GMOCK OFF)
add_subdirectory(${PROJECT_DEPENDENCIES_DIR}/gtest)
set(gtest_SOURCE_DIR ${PROJECT_DEPENDENCIES_DIR}/gtest/googletest)
add_subdirectory(test)

################################################################################
# include dirs 
################################################################################
include_directories(3party/msgpackc/include)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${PROJECT_LIBRARIES_DIR})

add_library(${PROJECT_NAME} ${PROJECT_SOURCE})

foreach(DEPENDENCY ${PROJECT_DEPENDENCIES})
    add_subdirectory(${PROJECT_DEPENDENCIES_DIR}/${DEPENDENCY})
endforeach(DEPENDENCY)

message("\nLink " ${PROJECT_NAME} " to libraries: " ${PROJECT_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${PROJECT_DEPENDENCIES})
