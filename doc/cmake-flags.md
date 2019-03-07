# Description
The library can be compiled for multiple platforms. This can be directed through passing the right flags to cmake.
A description of the most important flags can be found in the following table.

| Flag | Values | Description |
| -- | -- | -- |
| -DCMAKE_BUILD_TYPE | Debug / Release | Tells the compiler to produce a release or debug build. The library can only be debugged with a debugger, if the build type is Debug |
| -DARCHITECTURE | EMBEDDED / X64 / X86 | Tells the compiler for which architecture he should compile the library. Currently 32 Bit, 64 Bit as well as embedded architectures are supported. For a embedded architecture, a matching toolchain has to be specified |
| -DCMAKE_C_FLAGS | Same flags as normally would be used | Tells the compiler which flags he should use to compile the library |
| -DSE__TESTING | ON / OFF | Enables / Disables the gtest framework |
| -DSE__BUILD_EXAMPLES | ON / OFF | Compiles the examples of in examples |
