cmake_minimum_required(VERSION 3.0.0)

set (CMAKE_CXX_STANDARD 11)

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

project(${PROJECT_NAME})

################################################################################
# Set options
################################################################################
#set(CMAKE_C_COMPILER gcc)
#set(CMAKE_CXX_COMPILER g++)

set(ARCHITECTURE_32BIT ON)

set(CMAKE_BUILD_TYPE DEBUG)

if(ARCHITECTURE_32BIT)
    message("-- 32Bit Architecture flag is on")
    set(CMAKE_C_FLAGS -m32)
    set(CMAKE_CXX_FLAGS -m32)
    set(MSGPACK_32BIT ON)
endif()

################################################################################
# Set options
################################################################################
set(PROJECT_SOURCE_DIR
    src
)
set(PROJECT_DEPENDENCIES_DIR
    3party
)
set(PROJECT_EXAMPLES_DIR
    examples
)
set(PROJECT_DEPENDENCIES
  msgpackc
)
set(PROJECT_INCLUDE_DIR
    include/Scope
)
set(PROJECT_TEST_DIR
    test
)
#######################################################################################################################
## Find all needed files
#######################################################################################################################
file( GLOB_RECURSE PROJECT_SOURCE "${PROJECT_SOURCE_DIR}/*.c" )
file( GLOB_RECURSE PROJECT_HEADER "${PROJECT_INCLUDE_DIR}/*.h" )

################################################################################
# GTest 
################################################################################
enable_testing()
set(BUILD_GMOCK OFF)
add_subdirectory(${PROJECT_DEPENDENCIES_DIR}/gtest)
set(gtest_SOURCE_DIR ${PROJECT_DEPENDENCIES_DIR}/gtest/googletest)
add_subdirectory(test)

################################################################################
# Include dirs and link libraries
################################################################################
include_directories(${PROJECT_DEPENDENCIES_DIR}/msgpackc/include)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${PROJECT_INCLUDE_DIR})

add_library(${PROJECT_NAME} STATIC ${PROJECT_SOURCE})

foreach(DEPENDENCY ${PROJECT_DEPENDENCIES})
    add_subdirectory(${PROJECT_DEPENDENCIES_DIR}/${DEPENDENCY})
endforeach(DEPENDENCY)

#message("\nLink " ${PROJECT_NAME} " to libraries: " ${PROJECT_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${PROJECT_DEPENDENCIES})

################################################################################
# Add examples
################################################################################
if(ADD_EXAMPLES)
  file( GLOB_RECURSE EXAMPLE_SOURCES "${PROJECT_EXAMPLES_DIR}/*/CMakeLists.txt")
  message("-- Examples will be added")
  get_filename_component(EXAMPLE_SUB_DIRS ${EXAMPLE_SOURCES} DIRECTORY)
  add_subdirectory(${EXAMPLE_SUB_DIRS}) 
endif()
################################################################################
# Install project 
################################################################################
message("-- Project will be installed to: ${CMAKE_INSTALL_PREFIX}")
install( DIRECTORY "${PROJECT_INCLUDE_DIR}/" DESTINATION "${CMAKE_INSTALL_PREFIX}/include" )
install( TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/libs/" COMPONENT library )
