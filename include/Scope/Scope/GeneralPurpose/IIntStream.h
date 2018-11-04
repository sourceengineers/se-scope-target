/*!****************************************************************************************************************************************
 * @file         IIntStream.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Streaming interface
 *               open: opens the channel to a given Int pointer
 *               length: returns the amount of data ready to be read
 *               read: returns a pointer to the beginning of the data array
 *               write: writes Ints into the stream
 *               writeData: writes a single data point into the stream
 *               readData: returns a single data point from the stream
 *               flush: flushs the stream
 *               close: closes the stream
 *
 *****************************************************************************************************************************************/

#ifndef IINTSTREAM_H_
#define IINTSTREAM_H_

#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct IIntStreamStruct* IIntStreamHandle;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct IIntStreamStruct {
    void* implementer;
    void (*open)(IIntStreamHandle self, gemmi_uint* stream, const size_t capacity);
    bool (*dataIsReady)(IIntStreamHandle self);
    const gemmi_uint (*readData)(IIntStreamHandle self);
    size_t (*length)(IIntStreamHandle self);
    void (*read)(IIntStreamHandle self, gemmi_uint* data, const size_t length);
    void (*writeData)(IIntStreamHandle self, const gemmi_uint data);
    void (*write)(IIntStreamHandle self, const gemmi_uint* data, const size_t length);
    void (*close)(IIntStreamHandle self);
    void (*flush)(IIntStreamHandle self);
} IIntStream ;

#endif
