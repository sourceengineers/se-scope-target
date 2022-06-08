# Description
The se-scope-target is a library, used to visualize and log data produced by microcontrollers. 
A set of commands allow the scope to be dynamically reconfigured during runtime.
It features multiple trigger modes, such as a continuous, edge detecting as well as one shot mode.

# Building
### Cmake
Once the dependencies are installed, download the repo to what ever folder and run:
```bash
git clone https://bitbucket.org/sourceengineers/se-scope-target.git
mkdir se-scope-target/build && cd se-scope-target/build 
cmake ..
make
sudo make install
```
By providing a toolchain file, the library will automatically be compile for the respective target architecture.

As example:
```bash
git clone --recursive https://bitbucket.org/sourceengineers/se-scope-target.git
cd se-scope-target
mkdir build && cd build 
cmake -DCMAKE_TOOLCHAIN_FILE=TOOLCHAIN_FILE_PATH -DCMAKE_C_FLAGS="COMPILER_FLAGS" ..
make
sudo make install
```
### Vendor IDE's
Other than building the project with CMake, the project currently also provides support for CubeIde, Keil and CCS.

## Dependencies
Besides the platform specific tools build tools as well as CMake, no further dependencies are required.
### Ubuntu & Debian
```bash
sudo apt install cmake build-essential
```

### Tests
Run the internal unit tests with:
```
...
cmake ..
make se-scope-test
ctest -VV
``` 

# Usage
TODO: Write examples and add or link them here
# Allocation
For embedded systems, dynamic resource allocation is often not possible. To support these cases, 
the scope avoids any sort of heap allocation after being built.
In future versions, a interface will be available to provide custom allocator in case the scope must be fully run on the stack.
The allocator used during the building process, will be able to be swapped out for a custom allocator. Thus calls to 
malloc can be entirely avoided of necessary.
