# Windows build instructions
It is possible to compile the library on Windows through a similar process as for \*nix systems.
Although it might be easier to generate the project file for the specific IDE, or just use the specific project file if its already present. To do this, check the doc for build-\*.
The rest of this description will explain how to use cmake and MinGW to compile the library.
## Dependencies
- You will need to install [MinGW](http://mingw.org/wiki/Install_MinGW) for either 32 or 64 bit, depending on what architecture you want to compile the library. Make sure that you installed the "mingw-make.exe" through the mingw installer, or CMake will not be able to generate a makefile.
- [CMake](https://cmake.org/download/)
- [Git](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git)
- [Python](https://www.python.org/)

Make sure that the path to the MinGW-make.exe as well as the one to cmake.exe are set up correctly.

If the compile or cmake command yield an error, try running the powershell as administrator and retry.
### Dev Tools
If you want to use the dev tools under tools, you also have to install python, as well as pip.
To install the dependencies run:
```bash
pip install msgpack pyserial
```
## Build for OS
Once the dependencies are installed, download the repo to what ever folder and run:
```bash
git clone --recurse-submodules -j8 https://bitbucket.org/sourceengineers/iot-scope-target.git
mkdir iot-scope-target/build && cd iot-scope-target/build
cmake.exe -G "MinGW Makefiles" -DARCHITECTURE=X64 ..
mingw64-make.exe install
```
### Tests
If you want to run tests, you have to set the cmake GEMMI_TESTING flag to ON. And afterwards run the ctest.
```
...
cmake -DARCHITECTURE=X64 -DGEMMI_TESTING=ON ..
make
ctest -VV
```
## Build for embedded
Building the library for embedded systems a bit more complicated, since a specific toolchain has to be specified.
The following procedure has to be followed.

1. Download the toolchain matching the platform you want to cross compile. As example the gcc-arm-none-eabi for ARM platforms.
2. Specify the toolchain in a separate file. A example of this can be found under examples/arm.
3. The rest of the procedure is largely the same as the one for unix systems. The only difference is, that the toolchain has to be linked to the CMake.
```bash
git clone --recurse-submodules -j8 https://bitbucket.org/sourceengineers/iot-scope-target.git
mkdir iot-scope-target/build && cd iot-scope-target/build
cmake.exe -G"MinGW Makefiles" -DARCHITECTURE=EMBEDDED -DCMAKE_TOOLCHAIN_FILE=TOOLCHAIN_FILE_PATH -DCMAKE_C_FLAGS="COMPILER_FLAGS" ..
mingw64-make.exe install
```
