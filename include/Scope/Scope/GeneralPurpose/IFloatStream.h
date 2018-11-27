/*!****************************************************************************************************************************************
 * @file         IFloatStream.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Streaming interface
 *               open: opens the channel to a given Float pointer
 *               length: returns the amount of data ready to be read
 *               read: returns a pointer to the beginning of the data array
 *               write: writes Floats into the stream
 *               writeData: writes a single data point into the stream
 *               readData: returns a single data point from the stream
 *               flush: flushs the stream
 *               close: closes the stream
 *
 *****************************************************************************************************************************************/

#ifndef IFLOATSTREAM_H_
#define IFLOATSTREAM_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct IFloatStreamStruct* IFloatStreamHandle;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct IFloatStreamStruct {
    void* implementer;
    void (*open)(IFloatStreamHandle self, float* stream, const size_t capacity);
    bool (*dataIsReady)(IFloatStreamHandle self);
    float (*readData)(IFloatStreamHandle self);
    size_t (*length)(IFloatStreamHandle self);
    void (*read)(IFloatStreamHandle self, float* data, const size_t length);
    void (*writeData)(IFloatStreamHandle self, const float data);
    void (*write)(IFloatStreamHandle self, const float* data, const size_t length);
    void (*close)(IFloatStreamHandle self);
    void (*flush)(IFloatStreamHandle self);
} IFloatStream ;

#endif
