/*!*****************************************************************************
 * @file         FramedIO.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of a communication interface for frame.
 *
 ******************************************************************************/

#ifndef FRAMEDIO_H
#define FRAMEDIO_H

#include "Scope/GeneralPurpose/IByteStream.h"
#include "Scope/GeneralPurpose/IRunnable.h"
#include "Scope/Communication/ITransceiver.h"

#include "Scope/Communication/ICommunicator.h"
#include "Scope/GeneralPurpose/IObserver.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __FramedIOPrivateData* FramedIOHandle;

/******************************************************************************
 Public functions
******************************************************************************/
/**
 * Generates the frame communication interface
 * @param callback Callback to the transmit function defined by the user
 * @param input
 * @param output
 * @return
 */
FramedIOHandle FramedIO_create(TransmitCallback callback, IByteStreamHandle input, IByteStreamHandle output);

/**
 * Returns the communicator interface
 * @param self
 * @return
 */
ICommunicatorHandle FramedIO_getCommunicator(FramedIOHandle self);

/**
 * Returns the transceiver interface
 * @param self
 * @return
 */
ITransceiverHandle FramedIO_getTransceiver(FramedIOHandle self);

/**
 * Writes the data to transmit into data
 * @param self
 * @param data Array to which the data will be written
 * @param length Length of the data array. If this is shorter than EthernetJson_amountOfTxDataPending,
 *               nothing will be written into data
 */
void FramedIO_getTxData(FramedIOHandle self, uint8_t* data, size_t length);

/**
 * Returns the amount of data pending to be sent
 * @param self
 * @return
 */
 size_t FramedIO_amountOfTxDataPending(FramedIOHandle self);

/**
 * Writes data into the object
 * @param self
 * @param data data to be written into the stream
 * @param length amount of data to be written into the stream
 */
bool FramedIO_putRxData(FramedIOHandle self, const uint8_t* data, size_t length);

/**
 * Resets the input stream
 * @param self
 */
void FramedIO_resetRx(FramedIOHandle self);

/**
 * Resets the output stream
 * @param self
 */
void FramedIO_resetTx(FramedIOHandle self);

/**
 * Deconstructor
 * @param self
 */
void FramedIO_destroy(FramedIOHandle self);

#endif


