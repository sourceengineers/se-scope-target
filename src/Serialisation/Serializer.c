/*!****************************************************************************************************************************************
 * @file         Serializer.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Serialisation/Serializer.h>
#include <stdlib.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __SerializerPrivateData {

    IRxTxRunnable rxTxRunnable;

    IRunnableHandle packer;
    IRunnableHandle unpacker;

} SerializerPrivateData;

/******************************************************************************
 Private functions
******************************************************************************/
static void runRx(IRxTxRunnableHandle runnable){
    SerializerHandle self = (SerializerHandle) runnable->handle;
    self->unpacker->run(self->unpacker);
}

static void runTx(IRxTxRunnableHandle runnable){
    SerializerHandle self = (SerializerHandle) runnable->handle;
    self->packer->run(self->packer);
}

/******************************************************************************
 Public functions
******************************************************************************/
SerializerHandle Serializer_create(IRunnableHandle packer, IRunnableHandle unpacker){

    SerializerHandle self = malloc(sizeof(SerializerPrivateData));

    self->packer = packer;
    self->unpacker = unpacker;
    self->rxTxRunnable.runRx = &runRx;
    self->rxTxRunnable.runTx = &runTx;

    return self;
}

IRxTxRunnableHandle Serializer_getRxTxRunnable(SerializerHandle self){
    return &self->rxTxRunnable;
}

void Serializer_destroy(SerializerHandle self){
    free(self);
    self = NULL;
}
