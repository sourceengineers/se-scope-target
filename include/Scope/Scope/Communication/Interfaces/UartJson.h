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
#include <Scope/Communication/CommunicationTypes.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __UartJsonPrivateData* UartJsonHandle;

/******************************************************************************
 Public functions
******************************************************************************/
UartJsonHandle UartJson_create(TransmitCallback callback, IByteStreamHandle input, IByteStreamHandle output);

ICommunicatorHandle UartJson_getCommunicator(UartJsonHandle self);

void UartJson_destroy(UartJsonHandle self);

#endif


