/*!****************************************************************************************************************************************
 * @file         IUnpacker.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Specifies an interface which must be used by a protocol to allows the scope to unpack
 *               input data.
 *               This allows to extend the scope for multiple protocols.
 *
 *****************************************************************************************************************************************/

#ifndef IUNPACKER_H_
#define IUNPACKER_H_

#include "Scope/GeneralPurpose/DataTypes.h"

/******************************************************************************
 Define interface handle datar
******************************************************************************/
typedef struct IUnpackerStruct* IUnpackerHandle;

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

    /**
     * Unpacks the data in the input stream
     * @param unpacker
     * @return
     */
    bool (* unpack)(IUnpackerHandle unpacker);

    /**
     * Returns the number of commands which were unpacked
     * @param unpacker
     * @return
     */
    size_t (* getNumberOfCommands)(IUnpackerHandle unpacker);

    /**
     * Returns the name of the command at index
     * @param unpacker
     * @param name string into which the name will be written into
     * @param maxLenght maximal length of the name field
     * @param index
     * @return
     */
    bool (* getNameOfCommand)(IUnpackerHandle unpacker, char* name, const int maxLenght, const int index);

    /**
     * Returns an int value
     * @param unpacker
     * @param information
     * @return
     */
    /* Functions to fetch the data from commands */
    ADDRESS_DATA_TYPE (* getIntFromCommand)(IUnpackerHandle unpacker, CommandFetchingInformation* information);

    /**
     * Returns a float value
     * @param unpacker
     * @param information
     * @return
     */
    float (* getFloatFromCommand)(IUnpackerHandle unpacker, CommandFetchingInformation* information);

    /**
     * Returns a bool value
     * @param unpacker
     * @param information
     * @return
     */
    bool (* getBoolFromCommand)(IUnpackerHandle unpacker, CommandFetchingInformation* information);

    /**
     * Fetches a string
     * @param unpacker
     * @param information
     * @param targetStr array to which the data gets written into
     * @param maxLenght max length of the targetStr field
     */
    void (* getStringFromCommand)(IUnpackerHandle unpacker, CommandFetchingInformation* information, char* targetStr,
                                  const int maxLenght);

    /**
     * Get amount of fields in a command
     * @param unpacker
     * @param commandName
     * @return
     */
    int (* getNumberOfFields)(IUnpackerHandle unpacker, const char* commandName);

    /**
     * Get the name of a field inside of a command
     * @param unpacker
     * @param commandName
     * @param fieldName
     * @param maxLenght
     * @param index
     * @return
     */
    bool (* getNameOfField)(IUnpackerHandle unpacker, const char* commandName, char* fieldName, const int maxLenght,
                            const int index);

} IUnpacker;

#endif
