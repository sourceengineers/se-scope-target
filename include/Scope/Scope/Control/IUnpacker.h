/*!****************************************************************************************************************************************
 * @file         IUnpacker.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Parser interface.
 *
 *               unpack: Unpacks the given data. Returns false if the parsing process failed
 *               dataPending: Offers the Controller a possibility to check if new data is ready to be read or not
 *               dataRead: This function has to be executed by the controller once the pending data was interpreted
 *               streamIsEmpty: Checks if data in the input stream is present or not
 *
 *               getNumberOfCommands: Returns the number of parser commands
 *               getNameOfCommand: Writes the name of the command, at the given index into the name field
 *               getNameOfField: Same as getNameOfCommand, but for the command fields
 *               getIntFromCommand: Returns the int value of a given command and field
 *               getFloatFromCommand: Returns the float value of a given command and field
 *               getBoolFromCommand: Returns the float value of a given command and field
 *               getStringFromCommand: Returns the float value of a given command and field
 *
 *
 *****************************************************************************************************************************************/

#ifndef IUNPACKER_H_
#define IUNPACKER_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define interface handle datar
******************************************************************************/
typedef struct IUnpackerStruct* IUnpackerHandle;

static const size_t MAX_COMMAND_LENGTH = 30;
static const size_t MAX_FIELD_LENGTH = 30;

/* Struct which is used to clean up the amount of parameters passed to the fetcher functions */
typedef struct{
    const char* commandName;
    const char* fieldName;
    bool isInArray;     // Set to true if the value is in a array. Like cf_addr as example
    size_t arrayIndex;  // Set the correct array index if isInArray is set to true
} CommandFetchingInformation;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct IUnpackerStruct{
    GenericReference handle;

    bool (* unpack)(IUnpackerHandle unpacker);

    bool (* dataPending)(IUnpackerHandle unpacker);
    void (* dataRead)(IUnpackerHandle unpacker);
    bool (* streamIsEmpty)(IUnpackerHandle unpacker);

    /* Functions to fetch commands and fields */
    size_t (* getNumberOfCommands)(IUnpackerHandle unpacker);
    bool (* getNameOfCommand)(IUnpackerHandle unpacker, char* name, const int maxLenght, const int index);
    /* Functions to fetch the data from commands */
    ADDRESS_DATA_TYPE (* getIntFromCommand)(IUnpackerHandle unpacker, CommandFetchingInformation* information);
    float (* getFloatFromCommand)(IUnpackerHandle unpacker, CommandFetchingInformation* information);
    bool (* getBoolFromCommand)(IUnpackerHandle unpacker, CommandFetchingInformation* information);
    void (* getStringFromCommand)(IUnpackerHandle unpacker, CommandFetchingInformation* information, char* targetStr,
                                  const int maxLenght);
    /* Functions to help the communication validators */
    int (* getNumberOfFields)(IUnpackerHandle unpacker, const char* commandName);
    bool (* getNameOfField)(IUnpackerHandle unpacker, const char* commandName, char* fieldName, const int maxLenght,
                            const int index);

} IUnpacker;

#endif
