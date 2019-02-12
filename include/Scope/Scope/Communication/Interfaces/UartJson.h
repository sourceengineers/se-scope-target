/*!*****************************************************************************
 * @file         UartJson.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 ******************************************************************************/

#ifndef UARTJSON_H
#define UARTJSON_H

#include <Scope/Communication/ICommunicator.h>
#include <Scope/GeneralPurpose/IByteStream.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __UartJsonPrivateData* UartJsonHandle;

typedef void(*UartTransmitCallback)(UartJsonHandle self);

/******************************************************************************
 Public functions
******************************************************************************/
UartJsonHandle UartJson_create(UartTransmitCallback callback, IByteStreamHandle input, IByteStreamHandle output);

ICommunicatorHandle UartJson_getCommunicator(UartJsonHandle self);

/* Reads length amount of data from the output stream.
 * If the requested length is more than the amount of data stored in the stream,
 * the function returns no new data */
void UartJson_getTxData(UartJsonHandle self, uint8_t* data, size_t length);

/* Returns how much data currently is stored in the output stream */
size_t UartJson_amountOfTxDataPending(UartJsonHandle self);

/* Resets the Input handler. This might be used, if a new command is expected */
void UartJson_resetRx(UartJsonHandle self);

/* Resets the output handler. This might be used, if a new command is expected */
void UartJson_resetTx(UartJsonHandle self);

/* Puts data into the input buffer */
void UartJson_putRxData(UartJsonHandle self, uint8_t* data, size_t length);

void UartJson_destroy(UartJsonHandle self);

#endif


