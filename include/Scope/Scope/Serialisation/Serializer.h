/*!*****************************************************************************
 * @file         Serializer.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 ******************************************************************************/

#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <Scope/GeneralPurpose/IRunnable.h>
#include <Scope/Control/IPacker.h>
#include <Scope/Control/IUnpacker.h>
#include <Scope/Communication/ICommunicator.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __SerializerPrivateData* SerializerHandle;

/******************************************************************************
 Public functions
******************************************************************************/
/* Constructor: Creates a new instance of the channel */
SerializerHandle Serializer_create(IPackerHandle packer, IUnpackerHandle unpacker);

IRunnableHandle Serializer_getRxRunnable(SerializerHandle self);

IRunnableHandle Serializer_getTxRunnable(SerializerHandle self);

void Serializer_attachCommunicationObserver(SerializerHandle self, IObserverHandle observer);

void Serializer_attachControlObserver(SerializerHandle self, IObserverHandle observer);

IObserverHandle Serializer_getPackObserver(SerializerHandle self);

IObserverHandle Serializer_getUnpackObserver(SerializerHandle self);

/* Deconstructor: Deletes the instance of the channel */
void Serializer_destroy(SerializerHandle self);

#endif


