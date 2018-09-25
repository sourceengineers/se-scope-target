
cmake_minimum_required(VERSION 3.0.0)

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

# Set the used libraries here !!!!!!!!!!!!!!!!!!!!!!!!!!!
set(PROJECT_LIBRARIES
    msgpack_parser
    Scope
    GeneralPurpose
    Communication
    Command
)

project(${PROJECT_NAME})

################################################################################
# GTest 
################################################################################
enable_testing()
add_subdirectory(libs/gtest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

################################################################################
# Project 
################################################################################

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE})

foreach(LIBRARY ${PROJECT_LIBRARIES})
    add_subdirectory(${PROJECT_LIBRARIES_DIR}/${LIBRARY})
endforeach(LIBRARY)

message("Link " ${PROJECT_NAME} " to libraries: " ${PROJECT_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${PROJECT_LIBRARIES})
