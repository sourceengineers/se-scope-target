/*!****************************************************************************************************************************************
 * @file         IByteStream.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Interface for an byte stream.
 *
 *****************************************************************************************************************************************/

#ifndef IBYTESTREAM_H_
#define IBYTESTREAM_H_

#include <se-lib-c/definition/SeLibCTypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct IByteStreamStruct* IByteStreamHandle;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct IByteStreamStruct{
    SeLibGenericReferece handle;

    /**
     * Checks if data is pending
     * @param self
     * @return True if data is pending, False if not
     */
    bool (* byteIsReady)(IByteStreamHandle self);

    /**
     * Returns the oldest data point
     * @param self
     * @return
     */
    uint8_t (* readByte)(IByteStreamHandle self);

    /**
     * Returns the amount of data in the stream
     * @param self
     * @return
     */
    size_t (* length)(IByteStreamHandle self);

    /**
     * Reads data in the stream
     * @param self
     * @param data Array into which the data has to be written
     * @param length Max size of the data array
     */
    void (* read)(IByteStreamHandle self, uint8_t* data, const size_t length);

    /**
     * Write one single data point into the stream
     * @param self
     * @param data
     */
    void (* writeByte)(IByteStreamHandle self, const uint8_t data);

    /**
     * Writes multiple data points into the stream
     * @param self
     * @param data Data that has to be written into the stream
     * @param length Amount of data that has to be written into the stream
     */
    void (* write)(IByteStreamHandle self, const uint8_t* data, const size_t length);

    /**
     * Flushes the stream
     * @param self
     */
    void (* flush)(IByteStreamHandle self);

    /**
     * Returns the maximum amount of data that can be safed in the stream
     * @param self
     */
    size_t (* capacity)(IByteStreamHandle self);

} IByteStream;

#endif
