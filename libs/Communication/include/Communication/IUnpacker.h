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
 *****************************************************************************************************************************************/

#ifndef IUNPACKER_H_
#define IUNPACKER_H_

#include <stdlib.h>
#include <stdbool.h>

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct IUnpackerStruct* IUnpackerHandle;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct IUnpackerStruct {
  void* implementer;
  bool (*unpack)(IUnpackerHandle iUnpacker, const char* data, const size_t length);
  void (*activateNewMessage)(IUnpackerHandle iUnpacker);
  
  const size_t (*getNumberOfCommands)(IUnpackerHandle iUnpacker);
  bool (*getNameOfCommand)(IUnpackerHandle iUnpacker, char* name, const int maxLenght, const int index);
  ssize_t (*getNumberOfFields)(IUnpackerHandle IUnpacker, const char* commandName);
  bool (*getNameOfField)(IUnpackerHandle iUnpacker, const char* commandName, char* fieldName, const int maxLenght, const int index);
  
  uint32_t (*getIntFromCommand)(IUnpackerHandle iUnpacker, const char* commandName, const char* fieldName);
  float (*getFloatFromCommand)(IUnpackerHandle iUnpacker,const char* commandName, const char* fieldName);
  bool (*getBoolFromCommand)(IUnpackerHandle iUnpacker,const char* commandName, const char* fieldName);
  void (*getStringFromCommand)(IUnpackerHandle iUnpacker,const char* commandName, const char* fieldName, char* targetStr, const int maxLenght);
} IUnpacker ;

#endif
