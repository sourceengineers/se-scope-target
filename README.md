# Description
This library aims to help developers debugging their code and embedded devices easier and more fluent.
It helps tracking values of multiple addresses for a variation of communication interfaces as well as protocols.

The target can be configured with the help of multiple pre defined commands, and sends data of the tracked addresses to the host computer. Through that, the data can be plotted on the host computer. This enables a type of debugging, which helps you determine the project actually does what it should and not just if its crashing or not.
# Building
The library is build upon cmake, and is therefore able to be compiled on pretty much anything.
For some more common embedded IDE's, the project contains already predefined project files. 

If the IDE is not yet supported, then the library has to be build with cmake.
If the library is to be compiled with cmake, the process is slightly different if the host system is a Windows or \*nix system. 

For detailed instructions, take a look at the doc folder and pick the documentation for what ever operating system the library is build on, or for what ever IDE is used.

## Cmake host platform
- [\*nix systems](https://bitbucket.org/sourceengineers/iot-scope-target/src/master/doc/build-nix.md)
- [Windows](https://bitbucket.org/sourceengineers/iot-scope-target/src/master/doc/build-windows.md)
## IDE's
- [Keil](https://bitbucket.org/sourceengineers/iot-scope-target/src/master/doc/build-keil)
# Usage
## Initialisation
Generate the scope. 
```c
#include <Gemmi.h>

/*
Definition of the Scope_create
ScopeHandle Scope_create(const size_t channelSize,
                         const size_t numberOfChannels,
                         const size_t maxNumberOfAddresses,
                         const COM_TYPE comType,
                         const PROT_TYPE protType,
                         const TIMESTAMPING_MODE timestampingMode,
                         ScopeTransmitCallback transmitCallback); */

/* Create the scope */
ScopeHandle scope = Scope_create(500, 3, 3, ETHERNET, JSON, TIMESTAMP_AUTOMATIC, print);
```
There are a few configurations that have to be set on the target side.

| Param | Values | Description |
| -- | -- | -- |
| comType | ETHERNET / UART | Adjusts the scope for the specified communication interface. This mainly affects data in the transport field |
| protType | JSON / MSGPACK | Adjusts the scope for the specified protocol |
| timestampingMode | TIMESTAMP_AUTOMATIC / TIMESTAMP_MANUAL | Configures the protocol to either timestamp automatically, or manually |

If a transmitCallback function is supplied, the scope will be able to send messages to the host automatically. 
This callback has to be a function in the form of:
```c
void ScopeTransmitCallback(IByteStreamHandle stream);
```
## Watch addresses
The watch addresses can be defined during compile time, and tell the host, what their names and addresses are during runtime.
This can be useful if the variables are not static.

```c
uint8_t var;
Scope_setAnnounceAddresses(scope,(const char*) "NAME_OF_VAR", &var, UINT8, 0);


```

# Protocol
# Desktop client
# License
