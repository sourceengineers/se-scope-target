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
#include <Scope/Communication/CommunicationTypes.h>
#include <Scope/Serialisation/IComValidator.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __EthernetJsonPrivateData* EthernetJsonHandle;

/******************************************************************************
 Public functions
******************************************************************************/
EthernetJsonHandle EthernetJson_create(TransmitCallback callback, IByteStreamHandle input, IByteStreamHandle output);

ICommunicatorHandle EthernetJson_getCommunicator(EthernetJsonHandle self);

IComValidatorHandle EthernetJson_getValidator(EthernetJsonHandle self);

void EthernetJson_destroy(EthernetJsonHandle self);

#endif


