cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 11)
# Set the Project name
set(SE_SCOPE_PROJECT_NAME se-scope)
project(${SE_SCOPE_PROJECT_NAME} C ASM)

################################################################################
# Set options
################################################################################
option(SE_SCOPE_BUILD_TESTS "Build the tests" ON)

# Inform the compiler to compile for 32 bit code
add_definitions(-DARCH_SIZE_32=1)

################################################################################
# Set options
################################################################################
set(SE_SCOPE_SOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(SE_SCOPE_PROJECT_DEPENDENCIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)
set(SE_SCOPE_PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/Scope)
set(SE_SCOPE_PROJECT_INCLUDE_DIR_PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public)
set(SE_SCOPE_PROJECT_EXT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext)

#######################################################################################################################
## Find all needed files
#######################################################################################################################
include(cmake/SourceFiles.cmake)

add_library(${SE_SCOPE_PROJECT_NAME} STATIC ${SE_SCOPE_SOURCES})
################################################################################
# GTest
################################################################################
if(SE_SCOPE_BUILD_TESTS)
    enable_testing()
    set(SE_SCOPE_PROJECT_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test)
    add_subdirectory(test)
endif()
################################################################################
# Include dirs and link libraries
################################################################################
option(nanopb_BUILD_GENERATOR OFF)
add_subdirectory(${SE_SCOPE_PROJECT_DEPENDENCIES_DIR}/nanopb)

#Se lib c
message("-- Adding se-lib-c")
add_subdirectory(${SE_SCOPE_PROJECT_EXT_DIR}/se-lib-c)
################################################################################
# Install project
################################################################################
TARGET_INCLUDE_DIRECTORIES(${SE_SCOPE_PROJECT_NAME}
        PUBLIC
        $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ext/se-lib-c/include>
        $<BUILD_INTERFACE:${SE_SCOPE_PROJECT_INCLUDE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${NANOPB_INCLUDE_DIRS}>
        $<BUILD_INTERFACE:${SE_SCOPE_PROJECT_DEPENDENCIES_DIR}/nanopb>
        )

target_link_libraries(${SE_SCOPE_PROJECT_NAME}
        PRIVATE
        $<BUILD_INTERFACE:protobuf-nanopb-static>
        $<BUILD_INTERFACE:se-lib-c>
        )

message("-- Project will be installed to: ${CMAKE_INSTALL_PREFIX}")
install(TARGETS ${SE_SCOPE_PROJECT_NAME} ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/" COMPONENT library)
install(DIRECTORY "${SE_SCOPE_PROJECT_INCLUDE_DIR_PUBLIC}/Scope" DESTINATION "${CMAKE_INSTALL_PREFIX}/include")
