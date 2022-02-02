# Create a file named options.cmake and populate with valid paths assigned to the following:
#
#     set(CG_TOOL_ROOT        "/opt/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.1.LTS")
#     set(DEVICE_SUPPORT_ROOT "/home/asdf/controlSUITE/device_support/F2807x/v210")
#     Or set them like in the example below
# Then do:
#     mkdir build && cd build
#     cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain-ti-c2000.cmake ..
#     make
# The ti example in the example folder has been compiled with:
#     cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/tolchains/ti-c2000.cmake \
#           -DCG_TOOL_ROOT=/opt/ccstudio/ccs/tools/compiler/ti-cgt-c2000_21.6.0.LTS \
#           -DDEVICE_SUPPORT_ROOT=/opt/ti/c2000/C2000Ware_3_04_00_00/device_support/f2806x \
#           -DGLOBAL_COMPILER_FLAGS="-v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 --advice:performance=all  -g --c11 --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --define=uint8_t=uint16_t --define=int8_t=int16_t"  ..

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

# set target system
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION  1)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# toolchain paths
find_program(TI_GCC             NAMES   cl2000    PATHS  ${CG_TOOL_ROOT}/bin    NO_DEFAULT_PATH)
find_program(TI_CXX             NAMES   cl2000    PATHS  ${CG_TOOL_ROOT}/bin    NO_DEFAULT_PATH)
find_program(TI_AS              NAMES   cl2000    PATHS  ${CG_TOOL_ROOT}/bin    NO_DEFAULT_PATH)
find_program(TI_AR              NAMES   ar2000    PATHS  ${CG_TOOL_ROOT}/bin    NO_DEFAULT_PATH)
find_program(TI_OBJCOPY         NAMES   ofd2000   PATHS  ${CG_TOOL_ROOT}/bin    NO_DEFAULT_PATH)
find_program(TI_OBJDUMP         NAMES   hex2000   PATHS  ${CG_TOOL_ROOT}/bin    NO_DEFAULT_PATH)
find_program(TI_SIZE            NAMES   size2000  PATHS  ${CG_TOOL_ROOT}/bin    NO_DEFAULT_PATH)
find_program(TI_LD              NAMES   cl2000    PATHS  ${CG_TOOL_ROOT}/bin    NO_DEFAULT_PATH)

# set executables settings
set(CMAKE_C_COMPILER    ${TI_GCC})
set(CMAKE_CXX_COMPILER  ${TI_CXX})
set(AS                  ${TI_AS})
set(AR                  ${TI_AR})
set(OBJCOPY             ${TI_OBJCOPY})
set(OBJDUMP             ${TI_OBJDUMP})
set(SIZE                ${TI_SIZE})
set(LD                  ${TI_LD})

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
