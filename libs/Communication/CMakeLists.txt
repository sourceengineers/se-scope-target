
cmake_minimum_required(VERSION 3.0.0)

# Set Library Name
# This has to be set for every module
# The module name also has to be set in the CmakeList in the root directory,
# for it to be compiled and included in the project
set(LIBRARY_NAME
  Communication
)

# Set all used dependencies
set(DEPENDENCIES  
#   GeneralPurpose
)

# Set Project Name
set(PROJECT_NAME
    Lib${LIBRARY_NAME}
)

################################################################################
# Include all Header Files
################################################################################
set(LIBRARY_HEADERS_DIR
    include/${LIBRARY_NAME}
)
file(GLOB LIBRARY_HEADER_FILES
  ${LIBRARY_HEADERS_DIR}/*.h
)
set(LIBRARY_HEADERS
    ${LIBRARY_HEADER_FILES}
)

################################################################################
# Include all Source Files
################################################################################
set(LIBRARY_SOURCE_DIR
    src
)

file(GLOB LIBRARY_SRC_FILES
  ${LIBRARY_SOURCE_DIR}/*.c
)

set(LIBRARY_SOURCE
   ${LIBRARY_SRC_FILES}
)

################################################################################
# Compile Library
################################################################################
project(${PROJECT_NAME})

add_library(${LIBRARY_NAME} STATIC
    ${LIBRARY_SOURCE}
    ${LIBRARY_HEADERS}
)

target_include_directories(${LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${LIBRARY_NAME}>
    $<INSTALL_INTERFACE:include/${LIBRARY_NAME}>
)

target_include_directories(${LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

################################################################################
## Add External libraries
################################################################################
target_link_libraries(${LIBRARY_NAME} ${DEPENDENCIES})

add_subdirectory(test)
