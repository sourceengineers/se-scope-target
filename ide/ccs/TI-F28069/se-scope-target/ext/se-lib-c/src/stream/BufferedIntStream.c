/*!****************************************************************************************************************************************
 * @file         BufferedIntStream.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <se-lib-c/stream/BufferedIntStream.h>
#include <se-lib-c/container/IntRingBuffer.h>
#include <se-lib-c/stream/IIntStream.h>

#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __BufferedIntStreamPrivateData{
    IIntStream parent;
    IntRingBufferHandle buffer;

} BufferedIntStreamPrivateData;

static bool dataIsReady(IIntStreamHandle stream);

static uint32_t readData(IIntStreamHandle stream);

static void readAll(IIntStreamHandle stream, uint32_t* data, const size_t length);

static size_t streamLength(IIntStreamHandle stream);

static void writeData(IIntStreamHandle stream, const uint32_t data);

static void writeAll(IIntStreamHandle stream, const uint32_t* data, const size_t length);

static void flush(IIntStreamHandle stream);

static size_t getCapacity(IIntStreamHandle stream);

/******************************************************************************
 Private functions
******************************************************************************/
static bool dataIsReady(IIntStreamHandle stream){
    BufferedIntStreamHandle self = (BufferedIntStreamHandle) stream->handle;

    if(IntRingBuffer_getNumberOfUsedData(self->buffer) > 0){
        return true;
    }else{
        return false;
    }
}

static uint32_t readData(IIntStreamHandle stream){
    BufferedIntStreamHandle self = (BufferedIntStreamHandle) stream->handle;

    uint32_t data;
    IntRingBuffer_read(self->buffer, &data, 1);

    return data;
}

static void readAll(IIntStreamHandle stream, uint32_t* data, const size_t length){
    BufferedIntStreamHandle self = (BufferedIntStreamHandle) stream->handle;

    IntRingBuffer_read(self->buffer, data, length);
}

static size_t streamLength(IIntStreamHandle stream){
    BufferedIntStreamHandle self = (BufferedIntStreamHandle) stream->handle;

    return IntRingBuffer_getNumberOfUsedData(self->buffer);
}

static void writeData(IIntStreamHandle stream, const uint32_t data){
    BufferedIntStreamHandle self = (BufferedIntStreamHandle) stream->handle;

    if(IntRingBuffer_write(self->buffer, &data, 1) == -1){
        uint32_t dump;
        IntRingBuffer_read(self->buffer, &dump, 1);
        IntRingBuffer_write(self->buffer, &data, 1);
    }
}

static void writeAll(IIntStreamHandle stream, const uint32_t* data, const size_t length){
    for(int i = 0; i < length; ++i){
        stream->writeData(stream, data[i]);
    }
}

static void flush(IIntStreamHandle stream){
    BufferedIntStreamHandle self = (BufferedIntStreamHandle) stream->handle;

    IntRingBuffer_clear(self->buffer);
}

static size_t getCapacity(IIntStreamHandle stream){
    BufferedIntStreamHandle self = (BufferedIntStreamHandle) stream->handle;

    return IntRingBuffer_getCapacity(self->buffer);
}

/******************************************************************************
 Public functions
******************************************************************************/

BufferedIntStreamHandle BufferedIntStream_create(size_t capacity){

    BufferedIntStreamHandle self = (BufferedIntStreamHandle) malloc(sizeof(BufferedIntStreamPrivateData));
    assert(self);
    self->buffer = IntRingBuffer_create(capacity);

    self->parent.handle = self;
    self->parent.dataIsReady = &dataIsReady;
    self->parent.readData = &readData;
    self->parent.length = &streamLength;
    self->parent.read = &readAll;
    self->parent.writeData = writeData;
    self->parent.write = writeAll;
    self->parent.flush = flush;
    self->parent.capacity = &getCapacity;

    return self;
}

void BufferedIntStream_destroy(BufferedIntStreamHandle self){
    IntRingBuffer_destroy(self->buffer);

    free(self);
    self = NULL;
}

IIntStreamHandle BufferedIntStream_getIIntStream(BufferedIntStreamHandle self){
    return &self->parent;
}
