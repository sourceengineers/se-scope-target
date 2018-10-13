/*!****************************************************************************************************************************************
 * @file         IPacker.h
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

#ifndef IPacker_H_
#define IPacker_H_

#include <stdlib.h>
#include <stdbool.h>
#include <Scope/Channel.h>
#include <ByteStream.h>

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct IPackerStruct* IPackerHandle;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct IPackerStruct {
  void* implementer;
  void (*pack)(IPackerHandle iPacker);

  void (*prepareChannel)(IPackerHandle iPacker, IFloatStreamHandle stream, const uint32_t channelId);
  void (*prepareTimeIncrement)(IPackerHandle iPacker, const uint32_t timeIncrement);
  void (*prepareTimestamp)(IPackerHandle iPacker, const uint32_t timestamp);
  void (*prepareTrigger)(IPackerHandle iPacker, const bool isTriggered, const uint32_t channelId, const uint32_t timestamp);
  void (*prepareFlowControl)(IPackerHandle iPacker, const char* flowcontrol);
  void (*prepareTransportData)(IPackerHandle iPacker);
  void (*print)(IPackerHandle iPacker);

  ByteStreamHandle (*getByteStream)(IPackerHandle iPacker);

  void (*reset)(IPackerHandle iPacker);
} IPacker ;

#endif
