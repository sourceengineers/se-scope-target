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

#ifndef IPACKER_H_
#define IPACKER_H_

#include <stdlib.h>
#include <stdbool.h>
#include <Scope/Channel.h>
#include <Scope/GeneralPurpose/ByteStream.h>
#include <Scope/GeneralPurpose/IIntStream.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct IPackerStruct* IPackerHandle;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct IPackerStruct {
  void* implementer;
  void (*pack)(IPackerHandle packer);

  void (*prepareChannel)(IPackerHandle packer, IFloatStreamHandle stream, const gemmi_uint channelId);
  void (*prepareTimeIncrement)(IPackerHandle packer, const gemmi_uint timeIncrement);
  void (*prepareTimestamp)(IPackerHandle packer, IIntStreamHandle timestamp);
  void (*prepareTrigger)(IPackerHandle packer, const bool isTriggered, const gemmi_uint channelId, const gemmi_uint timestamp);
  void (*prepareFlowControl)(IPackerHandle packer, const char* flowcontrol);
  void (*prepareAddressAnnouncement)(IPackerHandle packer, const char* name, const char* type, const gemmi_uint address);

  IByteStreamHandle (*getByteStream)(IPackerHandle packer);

  void (*reset)(IPackerHandle packer);
} IPacker ;

#endif
