cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 11)

# Set the Project name
set(PROJECT_NAME
        se-scope
        )
################################################################################
# Set options
################################################################################
if (ARCHITECTURE STREQUAL "X86")

    # This option is used if the tests should be executed on a host machine,
    # supporting 32 Bit architectures. This is useful since the micro controllers
    # will probably run on 32 bit, but there are only few operating systems support this option
    message("-- x86 architecture is on")
    set(CMAKE_C_FLAGS -m32)
    set(CMAKE_CXX_FLAGS "-m32 -fpermissive")

    set(SE_TESTING ON)

    # Inform the compiler to compile for 32 bit code
    add_definitions(-DARCH_SIZE_32=1)

elseif (ARCHITECTURE STREQUAL "X64" OR ARCHITECTURE STREQUAL "")

    # X64 Host machines should be able to run tests as well as examples
    message("-- x64 architecture is on")

    set(SE_TESTING ON)

    # Inform the compiler to compile for 64 bit code
    add_definitions(-DARCH_SIZE_32=0)

    set(CMAKE_MACOSX_RPATH 1)

elseif (ARCHITECTURE STREQUAL "EMBEDDED")
    message("-- EMBEDDED architecture is on")

    # Disable testing and examples because they will not run on the
    # host with a embedded compiler anyway
    set(SE_TESTING OFF)

    # Inform the compiler to compile for 32 bit code
    add_definitions(-DARCH_SIZE_32=1)

endif ()

project(${PROJECT_NAME} C ASM)

################################################################################
# Set options
################################################################################
set(PROJECT_SOURCE_DIR
        src
        )
set(PROJECT_DEPENDENCIES_DIR
        3rdparty
        )
set(PROJECT_EXAMPLES_DIR
        examples
        )
set(PROJECT_DEPENDENCIES
        jsmn
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
file(GLOB_RECURSE PROJECT_SOURCE "${PROJECT_SOURCE_DIR}/*.c")
file(GLOB_RECURSE PROJECT_HEADER "${PROJECT_INCLUDE_DIR}/*.h")
################################################################################
# GTest
################################################################################
if (SE_TESTING)
    enable_testing()
    set(BUILD_GMOCK ON)
    add_subdirectory(${PROJECT_DEPENDENCIES_DIR}/gtest)
    set(gtest_SOURCE_DIR ${PROJECT_DEPENDENCIES_DIR}/gtest/googletest)
    add_subdirectory(test)
    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
endif ()
################################################################################
# Include dirs and link libraries
################################################################################
include_directories("${PROJECT_DEPENDENCIES_DIR}/jsmn"
        "${PROJECT_INCLUDE_DIR}")


add_library(${PROJECT_NAME} STATIC ${PROJECT_SOURCE})

foreach (DEPENDENCY ${PROJECT_DEPENDENCIES})
    add_subdirectory(${PROJECT_DEPENDENCIES_DIR}/${DEPENDENCY})
endforeach (DEPENDENCY)

target_link_libraries(${PROJECT_NAME} ${PROJECT_DEPENDENCIES})
################################################################################
# Add examples
################################################################################
#if (SE_BUILD_EXAMPLES)
#    add_subdirectory(examples/dynamic_example)
#    add_subdirectory(examples/static_example)
#endif ()
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
