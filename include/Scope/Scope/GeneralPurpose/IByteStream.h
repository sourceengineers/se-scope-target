/*!****************************************************************************************************************************************
 * @file         IByteStream.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Streaming interface
 *               open: opens the channel to a given byte pointer
 *               length: returns the amount of bytes ready to be read
 *               read: returns a pointer to the beginning of the byte array
 *               write: writes bytes into the stream
 *               writeByte: writes a single byte into the stream
 *               readByte: returns a single byte from the stream
 *               flush: flushs the stream
 *               close: closes the stream
 *
 *****************************************************************************************************************************************/

#ifndef IByteStream_H_
#define IByteStream_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct IByteStreamStruct* IByteStreamHandle;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct IByteStreamStruct {
  void* implementer;
  void (*open)(IByteStreamHandle bytestream, uint8_t* data, const size_t capacity);
  bool (*byteIsReady)(IByteStreamHandle self);
  const uint8_t (*readByte)(IByteStreamHandle self);
  size_t (*length)(IByteStreamHandle self);
  void (*read)(IByteStreamHandle self, uint8_t* data, const size_t length);
  void (*writeByte)(IByteStreamHandle self, const uint8_t data);
  void (*write)(IByteStreamHandle self, const uint8_t* data, const size_t length);
  void (*close)(IByteStreamHandle self);
  void (*flush)(IByteStreamHandle self);
} IByteStream ;

#endif
