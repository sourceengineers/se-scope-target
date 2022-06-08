# Description
The se-scope-target is a library, used to visualize and log data produced by microcontrollers on host computer over any
communication interface. 
It features multiple trigger modes for data aggregation, such as a continuous, edge detecting as well as a logging functionality. 
# Usage
TODO: Write examples and add or link them here

# Building
## Cmake
Once the dependencies are installed, download the repo to what ever folder and run:
```bash
git clone https://bitbucket.org/sourceengineers/se-scope-target.git
mkdir se-scope-target/build && cd se-scope-target/build 
cmake ..
make
sudo make install
```
By providing a toolchain file, the library will automatically be compiled for the respective target architecture.

As example:
```bash
git clone --recursive https://bitbucket.org/sourceengineers/se-scope-target.git
cd se-scope-target
mkdir build && cd build 
cmake -DCMAKE_TOOLCHAIN_FILE=TOOLCHAIN_FILE_PATH -DCMAKE_C_FLAGS="COMPILER_FLAGS" ..
make
sudo make install
```
### Dependencies
Besides the platform specific tools build tools as well as CMake, no further dependencies are required.
#### Ubuntu & Debian
```bash
sudo apt install cmake build-essential
```

#### Tests
Run the internal unit tests with:
```
...
cmake ..
make se-scope-test
ctest -VV
``` 
## Vendor IDE's
Other than building the project with CMake, the project currently also provides support for CubeIde, Keil and CCS.
