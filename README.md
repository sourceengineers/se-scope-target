# Description
This library aims to help developers debugging their code and embedded devices easier and more fluent.
It helps tracking values of multiple addresses for a variation of communication interfaces as well as protocols.

The target can be configured with the help of multiple pre defined commands, and sends data of the tracked addresses to the host computer. Through that, the data can be plotted on the host computer. This enables a type of debugging, which helps you determine the project actually does what it should and not just if its crashing or not.
# Building
The library is build upon cmake, and is therefore able to be compiled on pretty much anything.
Additionally there is a release package which contains the sources, to be included into what ever IDE and project is used.

If the library is to be compiled with cmake, the process is slightly different if the host system is a Windows or \*nix system. 

For detailed instructions, take a look at the doc folder and pick the documentation for what ever operating system the library is build on, or for what ever IDE is used.

## Cmake host platform
- [\*nix systems](https://bitbucket.org/sourceengineers/iot-scope-target/src/master/doc/build-nix.md)
- [Windows](https://bitbucket.org/sourceengineers/iot-scope-target/src/master/doc/build-windows.md)
## IDE's
- [IDE's](https://bitbucket.org/sourceengineers/iot-scope-target/src/master/doc/build-ide.md)
# Usage
The Scope can either be controlled through a number of commands, which are sent to the target, or by corresponding functions on the target itself.

The following chapter explains all the functions, which are available on the target itself and cannot be controlled through a command.

Out of these functions, Scope_create() is the only one needed. The other two are optionally.
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
| timestampingMode | TIMESTAMP_AUTOMATIC / TIMESTAMP_MANUAL | Configures the protocol to either timestamp automatically, or manually. In the manual mode, the index passed to the ev_poll command will be used. In the automatic mode, the scope will timestamp itself, according to configured timestamp increment. |

If a transmitCallback function is supplied, the scope will be able to send messages to the host automatically. 
This callback has to be a function in the form of:
```c
void ScopeTransmitCallback(IByteStreamHandle stream);
```
## Watch addresses
The watch addresses can be defined during compile time, and tell the host, what their names and addresses are during runtime.
This can be useful if the variables are not static.

```c

/*void Scope_addAnnounceAddresses(ScopeHandle self, const char* name, const void* address,
                             const DATA_TYPES type,
                             const gemmi_uint addressId);*/

/* Registers the addresses */
uint8_t var;
Scope_addAnnounceAddresses(scope,(const char*) "NAME_OF_VAR", &var, UINT8, 0);

/* Announces the addresses to the host. This only works if the transmitCallback is set correctly */
Scope_announceAddresses(scope);
```

| Param | Values | Description |
| -- | -- | -- |
| type | UINT8 / UINT16 / UINT32 / FLOAT | Registers what type of address should be announce. |
| addressId | int | Tells the scope on which index it should save the address. This index can not exceed the maxNumberOfAddresses |

## Commands
It is possible to control the scope entirely through commands send by the host. 
To see all available commands are and their functions, check the [protocol](https://bitbucket.org/sourceengineers/iot-scope-doc/src/master/Protocol.md) description.
For this to work, the Scope_receiveData() function has to be used.

Alternatively, every command has a corresponding function defined in Scope.h. For a detailed description of the corresponding functions check this [page](https://bitbucket.org/sourceengineers/iot-scope-target/src/master/doc/command-api.md).
### Command execution
If commands should be sent to the scope, the Scope_receiveData() function has to be used.
This function reads the data from the input stream and interprets it. Meaning, the data has to be received, written into the input stream and afterwards, Scope_receiveData(scope) can be called.

```c
IByteStreamHandle input_stream = Scope_getInputStream(scope);

/* Feed data in to the stream. Through what ever communication interface needed.
    As example through scanf: */
char data[100];
scanf(&data);

input_stream->write(input_stream, data, 100);

/* After the data is written to the stream, the command function can be executed */
Scope_receiveData(scope);
```
# Protocol
The host and the target are communicating through a custom defined protocol. 
The specification can be checked, in the dedicated [documentation](https://bitbucket.org/sourceengineers/iot-scope-doc/src/master/Protocol.md).
This protocol will be parsed in what ever protocol is chosen to be sent between host and target.
# Desktop client
A desktop client which is able to control the target and display the data, is currently under work.
For the moment, the developer tools in tools have to be used to communicate with the target have to be used. These will be extended before the work at the desktop client continue.
The progress of the client can be tracked in its respective [repo](https://bitbucket.org/sourceengineers/iot-scope/src/schuepbs/).
# License
