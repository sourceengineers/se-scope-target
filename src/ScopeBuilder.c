/*!****************************************************************************************************************************************
 * @file         ScopeBuilder.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/ScopeBuilder.h>
#include <Scope/Serialisation/JsonParser/JsonPacker.h>
#include <Scope/Serialisation/JsonParser/JsonUnpacker.h>
#include <Scope/Communication/Interfaces/EthernetJson.h>
/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __ScopeBuilderPrivateData {


    BufferedByteStreamHandle inputStream;
    BufferedByteStreamHandle outputStream;

    EthernetJsonHandle ethernetJson;

    JsonPackerHandle packer;
    JsonUnpackerHandle unpacker;

    ScopeHandle scope;
    ControllerHandle controller;
    SerializerHandle serializer;
    CommunicatorHandle communicator;

} ScopeBuilderPrivateData;

/******************************************************************************
 Private functions
******************************************************************************/

/* Constructor: Creates a new instance of the channel */
ScopeBuilderHandle ScopeBuilder_create(TransmitCallback callback, uint32_t* timestampReference){

    ScopeBuilderHandle self = malloc(sizeof(ScopeBuilderPrivateData));

    // TODO: The sizes have to be adjustable from outside the builder
    size_t outputBufferSize = JsonPacker_calculateBufferSize(3,3,200);
    size_t inputBufferSize = JsonUnpacker_calculateBufferSize();

    self->inputStream = BufferedByteStream_create(inputBufferSize);
    self->outputStream = BufferedByteStream_create(outputBufferSize);

    // TODO: Packer and unpacker have to be adjustable from outside the builder
    self->unpacker = JsonUnpacker_create(BufferedByteStream_getIByteStream(self->inputStream));
    self->packer = JsonPacker_create(3, 3, BufferedByteStream_getIByteStream(self->outputStream));

    // TODO: All Parameters of the scope have to be adjustable
    self->scope = Scope_create(200, 3, 3, timestampReference);

    self->ethernetJson = EthernetJson_create(callback, \
                                        BufferedByteStream_getIByteStream(self->inputStream), \
                                        BufferedByteStream_getIByteStream(self->outputStream));

    self->controller = Controller_create(Scope_getIScope(self->scope), JsonPacker_getIPacker(self->packer), \
            JsonUnpacker_getIUnpacker(self->unpacker));
    self->serializer = Serializer_create(JsonPacker_getIPacker(self->packer), \
            JsonUnpacker_getIUnpacker(self->unpacker));
    self->communicator = Communicator_create(EthernetJson_getCommunicator(self->ethernetJson));

    return self;
}


ScopeObject ScopeBuilder_build(ScopeBuilderHandle self){

    ScopeObject obj;

    obj.scope = self->scope;
    obj.input = BufferedByteStream_getIByteStream(self->inputStream);
    obj.runScope = Scope_getIRunnable(self->scope);
    obj.runControlRx = Controller_getRxRunnable(self->controller);
    obj.runControlTx = Controller_getTxRunnable(self->controller);
    obj.runSerializerRx = Serializer_getRxRunnable(self->serializer);
    obj.runSerializerTx = Serializer_getTxRunnable(self->serializer);
    obj.runCommunicationRx = Communicator_getRxRunnable(self->communicator);
    obj.runCommunicationTx = Communicator_getTxRunnable(self->communicator);

    return obj;
}

/* Deconstructor: Deletes the instance of the channel */
void ScopeBuilder_destroy(ScopeBuilderHandle self){


}