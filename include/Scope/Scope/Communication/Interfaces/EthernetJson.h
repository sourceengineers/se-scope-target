/*!*****************************************************************************
 * @file         EthernetJson.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of a communication interface for ethernet.
 *
 ******************************************************************************/

#ifndef ETHERNETJSON_H
#define ETHERNETJSON_H

#include <Scope/Communication/ICommunicator.h>
#include <Scope/GeneralPurpose/IByteStream.h>
#include <Scope/GeneralPurpose/IObserver.h>
#include <Scope/GeneralPurpose/IRunnable.h>

#include <stdint.h>
#include <stddef.h>
#include <stdint.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __EthernetJsonPrivateData* EthernetJsonHandle;

typedef void(*EthernetTransmitCallback)(EthernetJsonHandle self);

/******************************************************************************
 Public functions
******************************************************************************/
/**
 * Generates the ethernet communication interface
 * @param callback Callback to the transmit function defined by the user
 * @param input
 * @param output
 * @return
 */
EthernetJsonHandle
EthernetJson_create(EthernetTransmitCallback callback, IByteStreamHandle input, IByteStreamHandle output);

/**
 * Returns the communicator interface
 * @param self
 * @return
 */
ICommunicatorHandle EthernetJson_getCommunicator(EthernetJsonHandle self);

/**
 * Writes the data to transmit into data
 * @param self
 * @param data Array to which the data will be written
 * @param length Length of the data array. If this is shorter than EthernetJson_amountOfTxDataPending,
 *               nothing will be written into data
 */
void EthernetJson_getTxData(EthernetJsonHandle self, uint8_t* data, size_t length);

/**
 * Returns the amount of data pending to be sent
 * @param self
 * @return
 */
size_t EthernetJson_amountOfTxDataPending(EthernetJsonHandle self);

/**
 * Writes data into the object
 * @param self
 * @param data data to be written into the stream
 * @param length amount of data to be written into the stream
 */
void EthernetJson_putRxData(EthernetJsonHandle self, uint8_t* data, size_t length);

/**
 * Deconstructor
 * @param self
 */
void EthernetJson_destroy(EthernetJsonHandle self);

#endif


