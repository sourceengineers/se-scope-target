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
    iot-scope-target
)
set(PROJECT_SOURCE_DIR
    src
)
set(PROJECT_SOURCE
    ${PROJECT_SOURCE_DIR}/main.c
)
set(PROJECT_LIBRARIES_DIR
    libs
)

#######################################################################################################################
## Custom Libraries
#######################################################################################################################
# Set the used libraries here !!!!!!!!!!!!!!!!!!!!!!!!!!!
set(PROJECT_LIBRARIES
    Scope
    GeneralPurpose
    Communication
    Command
    MsgpackParser
    msgpackc
)

project(${PROJECT_NAME})

################################################################################
# GTest 
################################################################################
enable_testing()
set(BUILD_GMOCK OFF)
add_subdirectory(libs/gtest)
set(gtest_SOURCE_DIR libs/gtest/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

################################################################################
# Project 
################################################################################

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE})

foreach(LIBRARY ${PROJECT_LIBRARIES})
    add_subdirectory(${PROJECT_LIBRARIES_DIR}/${LIBRARY})
endforeach(LIBRARY)

message("\nLink " ${PROJECT_NAME} " to libraries: " ${PROJECT_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${PROJECT_LIBRARIES})
