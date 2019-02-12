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

#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct IFloatStreamStruct* IFloatStreamHandle;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct IFloatStreamStruct{
    GenericReference handle;

    bool (* dataIsReady)(IFloatStreamHandle self);

    float (* readData)(IFloatStreamHandle self);

    size_t (* length)(IFloatStreamHandle self);

    /**
     *
     * @param data array into which the data gets written. The size of this should be what ever "length" returned
     * @param length Amount of bytes which should be read
     */
    void (* read)(IFloatStreamHandle self, float* data, const size_t length);

    void (* writeData)(IFloatStreamHandle self, const float data);

    /**
     *
     * @param data Data which should be written into the Stream
     * @param length Amount of data which should be written
     */
    void (* write)(IFloatStreamHandle self, const float* data, const size_t length);

    void (* flush)(IFloatStreamHandle self);
} IFloatStream;

#endif
