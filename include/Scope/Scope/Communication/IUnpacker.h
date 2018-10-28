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

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct IUnpackerStruct* IUnpackerHandle;

typedef struct {
    char* commandName;
    char* fieldName;
    bool isInArray;
    size_t arrayIndex;
} CommandFetchingInformation;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct IUnpackerStruct {
  void* implementer;
  bool (*unpack)(IUnpackerHandle iUnpacker, const char* data, const size_t length);
  void (*activateNewMessage)(IUnpackerHandle iUnpacker);

  /* Functions to fetch commands and fields */
  const size_t (*getNumberOfCommands)(IUnpackerHandle iUnpacker);
  bool (*getNameOfCommand)(IUnpackerHandle iUnpacker, char* name, const int maxLenght, const int index);
  ssize_t (*getNumberOfFields)(IUnpackerHandle IUnpacker, const char* commandName);
  bool (*getNameOfField)(IUnpackerHandle iUnpacker, const char* commandName, char* fieldName, const int maxLenght, const int index);

  /* Functions to fetch the data from commands */
  uint32_t (*getIntFromCommand)(IUnpackerHandle iUnpacker, CommandFetchingInformation* information);
  float (*getFloatFromCommand)(IUnpackerHandle iUnpacker, CommandFetchingInformation* information);
  bool (*getBoolFromCommand)(IUnpackerHandle iUnpacker, CommandFetchingInformation* information);
  void (*getStringFromCommand)(IUnpackerHandle iUnpacker, CommandFetchingInformation* information, char* targetStr, const int maxLenght);

  /* Functions to help the communication validators */
  size_t (*getLengthOfCheck)(IUnpackerHandle iUnpackHandler);
  size_t (*getLengthOfBytesToCheck)(IUnpackerHandle iUnpackHandler);
  void (*getBytesToCheck)(IUnpackerHandle iUnpackHandler, uint8_t* data);
  void (*getCheck)(IUnpackerHandle iUnpackHandler, uint8_t* checkData);
} IUnpacker ;

#endif
