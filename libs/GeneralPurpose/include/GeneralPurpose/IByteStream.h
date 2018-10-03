/*!****************************************************************************************************************************************
 * @file         IByteStream.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Streaming interface
 *               open: opens the channel to a given byte pointer
 *               size: returns the amount of bytes ready to be read
 *               read: returns a pointer to the beginning of the byte array
 *               write: writes bytes into the stream
 *               close: closes the stream
 *
 *****************************************************************************************************************************************/

#ifndef IByteStream_H_
#define IByteStream_H_

#include <unistd.h>

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct IByteStreamStruct* IByteStreamHandle;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct IByteStreamStruct {
  void* implementer;
  void (*open)(IByteStreamHandle self, uint8_t* stream);
  ssize_t (*size)(IByteStreamHandle self);
  const uint8_t*(*read)(IByteStreamHandle self);
  ssize_t (*write)(IByteStreamHandle self, const uint8_t* data, const size_t length);
  void (*close)(IByteStreamHandle self);
} IByteStream ;

#endif
