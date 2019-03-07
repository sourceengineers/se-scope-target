# \*nix build instructions
## Dependencies
Besides the platform specific tools build tools as well as CMake, no further dependencies are required.
### Ubuntu & Debian
```bash
sudo apt install cmake build-essential git clang clang++ python3 python3-pip
```
### macOS
```bash
brew install cmake llvm git python3
```
### Dev Tools
If you want to use the dev tools under tools, you also have to install python dependencies.
```bash
pip install pyserial
```
## Build for OS
Once the dependencies are installed, download the repo to what ever folder and run:
```bash
git clone --recurse-submodules -j8 https://bitbucket.org/sourceengineers/iot-scope-target.git
mkdir iot-scope-target/build && cd iot-scope-target/build 
cmake -DARCHITECTURE=X64 ..
make
sudo make install
```
The library will now be installed to /usr/local/lib und /usr/local/include
### Tests
If you want to run tests, you have to set the cmake SE_TESTING flag to ON. And afterwards run the ctest.
```
...
cmake -DARCHITECTURE=X64 -DSE_TESTING=ON ..
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
cmake -DARCHITECTURE=EMBEDDED -DCMAKE_TOOLCHAIN_FILE=TOOLCHAIN_FILE_PATH -DCMAKE_C_FLAGS="COMPILER_FLAGS" ..
make
sudo make install
```
The libraries will now again be installed to /usr/local/lib und /usr/local/include
