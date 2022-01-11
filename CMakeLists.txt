cmake_minimum_required(VERSION 3.5)

################################################################################
# Options
################################################################################
option(TARGET_64BIT_SUPPORT OFF) # Set to true if the target architecture is 64 bits
option(TARGET_TI_SUPPORT OFF) # Set to true if the target is from TI

# 3rd party
option(nanopb_BUILD_GENERATOR OFF)

################################################################################
# Version
################################################################################
set(SE_SCOPE se-scope)
project(${SE_SCOPE} C ASM)

set(SE_SCOPE_MAJOR_VERSION 0)
set(SE_SCOPE_MINOR_VERSION 4)
set(SE_SCOPE_PATCH_VERSION 0)
set(SE_SCOPE_VERSION
        ${SE_SCOPE_MAJOR_VERSION}.${SE_SCOPE_MINOR_VERSION}.${SE_SCOPE_PATCH_VERSION})

################################################################################
# Sources and files
################################################################################
set(SE_SCOPE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(SE_SCOPE_SOURCES_IN cmake/SeScopeSources.cmake)
set(SE_SCOPE_CONFIG_IN cmake/SeScopeConfig.cmake.in)
set(SE_SCOPE_CONFIG_VERSION_IN cmake/SeScopeConfigVersion.cmake.in)
# Include the lib source files files
include(${SE_SCOPE_SOURCES_IN})
#Add GNU paths
include(GNUInstallDirs)

################################################################################
# Set options
################################################################################
set(SE_SCOPE_PROJECT_DEPENDENCIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)
set(SE_SCOPE_PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/Scope  ${SE_SCOPE_PROJECT_DEPENDENCIES_DIR}/nanopb)
set(SE_SCOPE_PROJECT_INCLUDE_DIR_PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public)
set(SE_SCOPE_PROJECT_EXT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext)

################################################################################
# Set options
################################################################################
# Inform the compiler to compile for 32 bit code

################################################################################
# Include dependency
################################################################################
add_subdirectory(ext/se-lib-c)

add_subdirectory(${SE_SCOPE_PROJECT_DEPENDENCIES_DIR}/nanopb)
################################################################################
# Generate the library
################################################################################
add_library(${SE_SCOPE} STATIC ${SE_SCOPE_SOURCES})

set_target_properties(${SE_SCOPE} PROPERTIES LINKER_LANGUAGE C)
TARGET_INCLUDE_DIRECTORIES(${SE_SCOPE}
        PUBLIC
        $<INSTALL_INTERFACE:include/Scope>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
        PRIVATE
        ${SE_SCOPE_PROJECT_INCLUDE_DIR}
        )

target_link_libraries(
        ${SE_SCOPE}
        PUBLIC
        se-lib::c
        protobuf-nanopb-static
)

add_library(se-lib::scope ALIAS ${SE_SCOPE})
target_link_libraries(${SE_SCOPE} INTERFACE se-lib::scope)

message("-- Compiling for ${CMAKE_SIZEOF_VOID_P} bit architecture")

if (TARGET_64BIT_SUPPORT)
    target_compile_definitions(${SE_SCOPE} PUBLIC SCOPE_TARGET_64BIT_SUPPORT)
endif()

if (TARGET_TI_SUPPORT)
    target_compile_definitions(${SE_SCOPE} PUBLIC SCOPE_TARGET_TI_SUPPORT)
endif()

target_compile_options(${SE_SCOPE} PRIVATE -Wall -Wextra -pedantic -Werror)

################################################################################
# Add testing
################################################################################
enable_testing()
set(SE_SCOPE_PROJECT_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test)
add_subdirectory(test EXCLUDE_FROM_ALL)

################################################################################
# Export the target
################################################################################
export(TARGETS ${SE_SCOPE} protobuf-nanopb-static NAMESPACE SeScope:: FILE "${PROJECT_BINARY_DIR}/SeScopeTargets.cmake")
export(PACKAGE ${SE_SCOPE})

################################################################################
# Install the library and headers
################################################################################
install(TARGETS ${SE_SCOPE} EXPORT SeScopeTargets ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT library)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/public/Scope" DESTINATION "${CMAKE_INSTALL_PREFIX}/include")

################################################################################
# Generate the configuration
################################################################################
set(CONF_INCLUDE_DIRS "${SE_SCOPE_PROJECT_INCLUDE_DIR_PUBLIC}")
configure_file(${SE_SCOPE_CONFIG_IN} "${PROJECT_BINARY_DIR}/SeScopeConfig.cmake" @ONLY)
set(CONF_INCLUDE_DIRS "${CMAKE_INSTALL_PREFIX}/include/Scope")
configure_file(${SE_SCOPE_CONFIG_IN} "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/SeScopeConfig.cmake" @ONLY)
configure_file(${SE_SCOPE_CONFIG_VERSION_IN} "${PROJECT_BINARY_DIR}/SeScopeConfigVersion.cmake" @ONLY)

################################################################################
# Install the generated files
################################################################################
install(FILES
        "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/SeScopeConfig.cmake"
        "${PROJECT_BINARY_DIR}/SeScopeConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SeScope COMPONENT dev)
install(EXPORT SeScopeTargets DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/SeScope" COMPONENT dev)