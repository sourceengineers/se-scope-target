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
    gemmi
)

################################################################################
# Set options
################################################################################

set(CMAKE_BUILD_TYPE DEBUG)

if(ARCHITECTURE STREQUAL "X86")

    # This option is used if the tests should be executed on a host machine,
    # supporting 32 Bit architectures. This is useful since the micro controllers
    # will probably run on 32 bit, but there are only few operating systems support this option
    message("-- x86 architecture is on")
    set(CMAKE_C_COMPILER /usr/bin/gcc)
    set(CMAKE_CXX_COMPILER /usr/bin/gcc++)
    set(CMAKE_C_FLAGS -m32)
    set(CMAKE_CXX_FLAGS "-m32 -fpermissive")

    set(MSGPACK_32BIT ON)
    set(GEMMI_TESTING ON)

    set(MSGPACK_ENABLE_SHARED OFF)
    set(BUILD_SHARED_LIBS OFF)

    set(GEMMI_BUILD_EXAMPLES ON)

    # Inform the compiler to compile for 32 bit code
    add_definitions(-DARCH_SIZE_32=1)

elseif(ARCHITECTURE STREQUAL "X64")

    # X64 Host machines should be able to run tests as well as examples
    message("-- x64 architecture is on")
    set(CMAKE_C_COMPILER /usr/bin/clang)
    set(CMAKE_CXX_COMPILER /usr/bin/clang++)

    set(GEMMI_BUILD_EXAMPLES ON)
    set(GEMMI_TESTING ON)

    # Inform the compiler to compile for 64 bit code
    add_definitions(-DARCH_SIZE_32=0)

    set(CMAKE_MACOSX_RPATH 1)

elseif(ARCHITECTURE STREQUAL "EMBEDDED")
    message("-- ARM architecture is on")

    # Setoptions so msgpack will compile correctly
    set(MSGPACK_BUILD_EXAMPLES OFF)
    set(MSGPACK_32BIT ON)
    set(MSGPACK_CXX11 OFF)
    set(MSGPACK_BOOST OFF)
    set(MSGPACK_ENABLE_SHARED OFF)
    set(MSGPACK_ENABLE_STATIC ON)
    set(BUILD_SHARED_LIBS OFF)
    set(MSGPACK_ENABLE_CXX OFF)

    # Disable testing and examples because they will not run on the
    # host with a embedded compiler anyway
    set(GEMMI_TESTING OFF)
    set(GEMMI_BUILD_EXAMPLES OFF)

    # Inform the compiler to compile for 32 bit code
    add_definitions(-DARCH_SIZE_32=1)

endif()


project(${PROJECT_NAME})

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
if(GEMMI_TESTING)
  enable_testing()
  set(BUILD_GMOCK ON)
  add_subdirectory(${PROJECT_DEPENDENCIES_DIR}/gtest)
  set(gtest_SOURCE_DIR ${PROJECT_DEPENDENCIES_DIR}/gtest/googletest)
  add_subdirectory(test)
  include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
endif()
################################################################################
# Include dirs and link libraries
################################################################################
include_directories(${PROJECT_DEPENDENCIES_DIR}/msgpackc/include)
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
if(GEMMI_BUILD_EXAMPLES)
  file( GLOB_RECURSE EXAMPLE_SOURCES "${PROJECT_EXAMPLES_DIR}/*/CMakeLists.txt")
  message("-- Examples will be added")
  foreach(EXAMPLE ${EXAMPLE_SOURCES})
      get_filename_component(EXAMPLE_SUB_DIRS ${EXAMPLE} DIRECTORY)
      add_subdirectory(${EXAMPLE_SUB_DIRS})
  endforeach(EXAMPLE)
endif()
################################################################################
# Install project
################################################################################
message("-- Project will be installed to: ${CMAKE_INSTALL_PREFIX}")
install( DIRECTORY "${PROJECT_INCLUDE_DIR}/" DESTINATION "${CMAKE_INSTALL_PREFIX}/include" )
install( TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/" COMPONENT library )
