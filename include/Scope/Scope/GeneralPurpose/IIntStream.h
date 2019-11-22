/*!****************************************************************************************************************************************
 * @file         IIntStream.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Interface for an int stream.
 *
 *****************************************************************************************************************************************/

#ifndef IINTSTREAM_H_
#define IINTSTREAM_H_

#include "Scope/GeneralPurpose/DataTypes.h"

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct IIntStreamStruct* IIntStreamHandle;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct IIntStreamStruct{
    GenericReference handle;

    /**
     * Checks if data is pending
     * @param self
     * @return True if data is pending, False if not
     */
    bool (* dataIsReady)(IIntStreamHandle self);

    /**
     * Returns the amount of data in the stream
     * @param self
     * @return
     */
    uint32_t (* readData)(IIntStreamHandle self);

    /**
     * Reads data in the stream
     * @param self
     * @param data Array into which the data has to be written
     * @param length Max size of the data array
     */
    size_t (* length)(IIntStreamHandle self);

    /**
     * Reads data in the stream
     * @param self
     * @param data Array into which the data has to be written
     * @param length Max size of the data array
     */
    void (* read)(IIntStreamHandle self, uint32_t* data, const size_t length);

    /**
     * Write one single data point into the stream
     * @param self
     * @param data
     */
    void (* writeData)(IIntStreamHandle self, const uint32_t data);

    /**
     * Writes multiple data points into the stream
     * @param self
     * @param data Data that has to be written into the stream
     * @param length Amount of data that has to be written into the stream
     */
    void (* write)(IIntStreamHandle self, const uint32_t* data, const size_t length);

    /**
     * Flushes the stream
     * @param self
     */
    void (* flush)(IIntStreamHandle self);

    /**
     * Returns the maximum amount of data that can be safed in the stream
     * @param self
     */
    size_t (* capacity)(IIntStreamHandle self);

} IIntStream;

#endif
