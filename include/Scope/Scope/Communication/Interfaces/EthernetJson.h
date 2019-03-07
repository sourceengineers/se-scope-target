/*!*****************************************************************************
 * @file         EthernetJson.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
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
EthernetJsonHandle
EthernetJson_create(EthernetTransmitCallback callback, IByteStreamHandle input, IByteStreamHandle output);

ICommunicatorHandle EthernetJson_getCommunicator(EthernetJsonHandle self);

void EthernetJson_getTxData(EthernetJsonHandle self, uint8_t* data, size_t length);

size_t EthernetJson_amountOfTxDataPending(EthernetJsonHandle self);

void EthernetJson_putRxData(EthernetJsonHandle self, uint8_t* data, size_t length);

void EthernetJson_destroy(EthernetJsonHandle self);

#endif


