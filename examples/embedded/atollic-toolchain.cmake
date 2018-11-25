# Sample toolchain file for building for ARM (w/hw float support) from an Ubuntu Linux system.
#
# Typical usage:
#    *) install cross compiler: `sudo apt-get install gcc-arm-linux-gnueabihf`
#    *) cd build
#    *) cmake -DCMAKE_TOOLCHAIN_FILE=~/Toolchain-Ubuntu-gnueabihf.cmake ..

# name of the target OS on which the built artifacts will run and the
# toolchain prefix. if target is an embedded system without an OS, set
# CMAKE_SYSTEM_NAME to `Generic`
set(CMAKE_SYSTEM_NAME Linux)
set(TOOLCHAIN_PREFIX arm-atollic-eabi)
SET(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

# cross compilers to use for C and C++
set(CMAKE_C_COMPILER /opt/Atollic_TrueSTUDIO_for_STM32_x86_64_9.1.0/ARMTools/bin/${TOOLCHAIN_PREFIX}-gcc)
set(CMAKE_CXX_COMPILER /opt/Atollic_TrueSTUDIO_for_STM32_x86_64_9.1.0/ARMTools/bin/${TOOLCHAIN_PREFIX}-g++)

include_directories(/home/schuepbs/Documents/Projects/iot-scope-target/build)

# target environment on the build host system
#   set 1st to dir with the cross compiler's C/C++ headers/libs
set(CMAKE_FIND_ROOT_PATH /opt/Atollic_TrueSTUDIO_for_STM32_x86_64_9.1.0/ARMTools)

# modify default behavior of FIND_XXX() commands to
# search for headers/libs in the target environment and
# search for programs in the build host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
