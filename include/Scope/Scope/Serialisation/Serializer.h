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
#include <Scope/GeneralPurpose/IRxTxRunnable.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __SerializerPrivateData* SerializerHandle;

/******************************************************************************
 Public functions
******************************************************************************/
/* Constructor: Creates a new instance of the channel */
SerializerHandle Serializer_create(IRunnableHandle packer, IRunnableHandle unpacker);

/* Deconstructor: Deletes the instance of the channel */
void Serializer_destroy(SerializerHandle self);

#endif


