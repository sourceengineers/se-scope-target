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
 *               activateNewMessage: Activates the last parsed message to be the new object, from which the data gets
*                                     fetched. This is used in case as example checksums are faulty.
 *               getNumberOfCommands: Returns the number of parser commands
 *               getNameOfCommand: Writes the name of the command, at the given index into the name field
 *               getNameOfField: Same as getNameOfCommand, but for the command fields
 *               getIntFromCommand: Returns the int value of a given command and field
 *               getFloatFromCommand: Returns the float value of a given command and field
 *               getBoolFromCommand: Returns the float value of a given command and field
 *               getStringFromCommand: Returns the float value of a given command and field
 *
 *               getLengthOfCheck: Returns the number of bytes present in check field of the received data
 *               getCheck: Returns the bytes of the check
 *               getLengthOfBytesToCheck: Returns the amount of data over which a check has to be conducted
 *               getBytesToCheck: Returns the bytes over which a check will be generated. This is the data in the
 *                                  payload field
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

/* Struct which is used to clean up the amount of parameters passed to the fetcher functions */
typedef struct {
    char* commandName;
    char* fieldName;
    bool isInArray;     // Set to true if the value is in a array. Like cf_addr as example
    size_t arrayIndex;  // Set the correct array index if isInArray is set to true
} CommandFetchingInformation;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct IUnpackerStruct {
  void* implementer;
  bool (*unpack)(IUnpackerHandle unpacker, const char* data, const size_t length);
  void (*activateNewMessage)(IUnpackerHandle unpacker);

  /* Functions to fetch commands and fields */
  size_t (*getNumberOfCommands)(IUnpackerHandle unpacker);
  bool (*getNameOfCommand)(IUnpackerHandle unpacker, char* name, const int maxLenght, const int index);

  /* Functions to fetch the data from commands */
  gemmi_uint (*getIntFromCommand)(IUnpackerHandle unpacker, CommandFetchingInformation* information);
  float (*getFloatFromCommand)(IUnpackerHandle unpacker, CommandFetchingInformation* information);
  bool (*getBoolFromCommand)(IUnpackerHandle unpacker, CommandFetchingInformation* information);
  void (*getStringFromCommand)(IUnpackerHandle unpacker, CommandFetchingInformation* information, char* targetStr, const int maxLenght);

  /* Functions to help the communication validators */
  size_t (*getLengthOfCheck)(IUnpackerHandle iUnpackHandler);
  size_t (*getLengthOfBytesToCheck)(IUnpackerHandle iUnpackHandler);
  void (*getBytesToCheck)(IUnpackerHandle iUnpackHandler, uint8_t* data);
  void (*getCheck)(IUnpackerHandle iUnpackHandler, uint8_t* checkData);
  int (*getNumberOfFields)(IUnpackerHandle IUnpacker, const char* commandName);
  bool (*getNameOfField)(IUnpackerHandle unpacker, const char* commandName, char* fieldName, const int maxLenght, const int index);

} IUnpacker ;

#endif
