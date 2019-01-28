/*!****************************************************************************************************************************************
 * @file         ScopeBuilder.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/ScopeBuilder.h>
#include <Scope/Core/Scope.h>
#include <Scope/Control/Controller.h>
#include <Scope/Serialisation/JsonParser/JsonPacker.h>
#include <Scope/Serialisation/JsonParser/JsonUnpacker.h>
/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __ScopeBuilderPrivateData {


} ScopeBuilderPrivateData;

/******************************************************************************
 Private functions
******************************************************************************/

/* Constructor: Creates a new instance of the channel */
ScopeBuilderHandle ScopeBuilder_create(){

    uint32_t timestampReference;

    // TODO: The sizes have to be adjustable from outside the builder
    size_t outputBufferSize = JsonPacker_calculateBufferSize(3,3,200);
    size_t inputBufferSize = JsonUnpacker_calculateBufferSize();

    BufferedByteStreamHandle inputStream = BufferedByteStream_create(inputBufferSize);
    BufferedByteStreamHandle outputStream = BufferedByteStream_create(outputBufferSize);

    // TODO: Packer and unpacker have to be adjustable from outside the builder
    JsonUnpackerHandle unpacker = JsonUnpacker_create(BufferedByteStream_getIByteStream(inputStream));
    JsonPackerHandle packer = JsonPacker_create(3, 3, BufferedByteStream_getIByteStream(outputStream));

    // TODO: All Parameters of the scope have to be adjustable
    ScopeHandle scope = Scope_create(200, 3, 3, &timestampReference);

    IScopeHandle iscope = Scope_getIScope(scope);

    /* The Controller will be the last class to be generated */
    ControllerHandle controller = Controller_create(iscope, JsonPacker_getIPacker(packer), JsonUnpacker_getIUnpacker(unpacker));
}

/* Deconstructor: Deletes the instance of the channel */
void ScopeBuilder_destroy(ScopeBuilderHandle self);