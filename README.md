# Description
The SE-Scope is a library, used to visualize and log data produced by microcrontollers. 
A set of commands allow the scope to be dynamically reconfigured during runtime.
It features mutliple trigger modes, such as a continous, edge detecting as well as one shot mode.

# Building
The library uses CMake as build tool.
Additionally there are pre configured project files for some supported IDE's such as Keil and CubeIDE. 
## Dependencies
Besides the platform specific tools build tools as well as CMake, no further dependencies are required.
### Ubuntu & Debian
```bash
sudo apt install cmake build-essential
```
## Build
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
git clone https://bitbucket.org/sourceengineers/se-scope-target.git
mkdir se-scope-target/build && cd se-scope-target/build 
cmake -DCMAKE_TOOLCHAIN_FILE=TOOLCHAIN_FILE_PATH -DCMAKE_C_FLAGS="COMPILER_FLAGS" ..
make
sudo make install
```

Other than with CMake building is currently supported for Keil and Atollic.

### Tests
Run the internal unit tests with:
```
...
cmake ..
make se-scope-test
ctest -VV
``` 

# Usage
The scope supports to being run in a parallel or sequential context. 
A example for the sequential context can be found below. For the parallel, platform specific mutexes have to be provided,
and the ScopeFramedThreadedStack_* instead of the ScopeFramedStack_* functions have to be used. 
```c
    // Configuration of the scope
    ScopeFramedStackConfig config = {
            // Maximum number of addresses that will be sent to the host
            .addressesInAddressAnnouncer = 4,
            // Attach the callback that will be called, once data is ready to be sent
            .callback = &transmit_data, // This function is optional. It will be called once data is ready to be sent
            // Attatch a callback which hast o be incremented manually according to the platform specific timestamps
            .timestamp = &timestamp,
            // Maximum amount of channels that can be configured
            .amountOfChannels = 3,
            // Polling timebase in seconds
            .timebase = 0.001f,
            // Channels size
            .sizeOfChannels = 50
    };

    // Provide a buffer for the logger
    ScopeFramedStackLogOptions logOptions = {
            .logByteStream = &bufferInterface,
    };

    // Create the scope according to the specified configuration
    scopeStack = ScopeFramedStack_create(config, logOptions);
    
    // Add addresses to the scope. These addresses will be sent to the host.
    // This allows the user to attach addresses to the channels from the GUI application
    // This step is optional, but is suggested. Otherwise the addresses have to be fetched from the .map file or
    // configured manually.
    // Be aware, that the maximum length of the name is limited to 25 characters to preserve ram space 
    AnnounceStorageHandle addressStorage = ScopeFramedStack_getAnnounceStorage(scopeStack);
    AnnounceStorage_addAnnounceAddress(addressStorage, "sinus", &sinus, SE_FLOAT);
    AnnounceStorage_addAnnounceAddress(addressStorage, "cosinus", &cosinus, SE_FLOAT);
```

The communication interface has to be implemented for each platform. 
To do so, data can be written into the scope by calling: 
```c
ITransceiverHandle transceiver = ScopeFramedStack_getTranscevier(scopeStack);
transceiver->put(me.transceiver, byteBuffer, bufferLength);
```
To read from the scope, the data can either be polled, or read in the callback attached to the scope. 
Data can be read from the scope by calling: 
```c
ITransceiverHandle transceiver = ScopeFramedStack_getTranscevier(scopeStack);
size_t dataPending = transceiver->outputSize(transceiver);
transceiver->get(transceiver, byteBuffer, dataPending)
```

Once the scope is setup, it can be ran by calling it in a infinite loop.
```c
while (1)
{   
    // Run the communication functions
    COMM_FUNTIONS();

    // Generate your data
    DATA_GENERATION();
    
    // Update your timestamp
    timestamp += 1;

    // Run the scope
    ScopeFramedStack_run(scopeStack);
}
```
For examples to use the scope in on various platforms, take a look at the examples provided in [the examples repository](https://bitbucket.org/sourceengineers/se-scope-target-examples/src/master/).
# Visualization
As a visualization GUI, the [SE-Scope](https://bitbucket.org/sourceengineers/se-scope-gui/src/master/) is suggested.

# Allocation
For embedded systems, dynamic resource allocation is often not possible. To support these cases, 
the scope avoids any sort of heap allocation after being built.
In future versions, a interface will be available to provide custom allocator in case the scope must be fully run on the stack.
The allocator used during the building process, will be able to be swapped out for a custom allocator. Thus calls to 
malloc can be entirely avoided of necessary.