# Create a file named options.cmake and populate with valid paths assigned to the following:
#
#     set(CG_TOOL_ROOT        "/opt/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.1.LTS")
#     set(DEVICE_SUPPORT_ROOT "/home/asdf/controlSUITE/device_support/F2807x/v210")
#
# Then do:
#     mkdir build && cd build
#     cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain-ti-c2000.cmake ..
#     make

if(NOT DEFINED CG_TOOL_ROOT)
    message(FATAL_ERROR "CG_TOOL_ROOT is not defined in options.cmake")
endif()

if(NOT DEFINED DEVICE_SUPPORT_ROOT)
    message(FATAL_ERROR "DEVICE_SUPPORT_ROOT is not defined in options.cmake")
endif()

if(NOT IS_DIRECTORY "${CG_TOOL_ROOT}")
    message(FATAL_ERROR "The specified directory CG_TOOL_ROOT does not exist: ${CG_TOOL_ROOT}")
endif()

if(NOT IS_DIRECTORY "${DEVICE_SUPPORT_ROOT}")
    message(FATAL_ERROR "The specified directory DEVICE_SUPPORT_ROOT does not exist: ${DEVICE_SUPPORT_ROOT}")
endif()
message(STATUS "Found a valid options.cmake")

INCLUDE(CMakeForceCompiler)
# this one is important
SET(CMAKE_SYSTEM_NAME Generic)
#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
CMAKE_FORCE_C_COMPILER(${CG_TOOL_ROOT}/bin/cl2000 "TI")
CMAKE_FORCE_CXX_COMPILER(${CG_TOOL_ROOT}/bin/cl2000 "TI")

# Add the default include and lib directories for tool chain
include_directories(
    ${CG_TOOL_ROOT}/include
    ${DEVICE_SUPPORT_ROOT}/headers/include
    ${DEVICE_SUPPORT_ROOT}/common/include
    )

# linker search paths
link_directories(
    ${CG_TOOL_ROOT}/lib
    ${DEVICE_SUPPORT_ROOT}/headers/cmd
    ${DEVICE_SUPPORT_ROOT}/common/cmd
    )

# where is the target environment 
SET(CMAKE_FIND_ROOT_PATH ${CG_TOOL_ROOT})

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
