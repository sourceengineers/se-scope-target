/*!****************************************************************************************************************************************
 * @file         BufferedFloatStream.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/GeneralPurpose/BufferedFloatStream.h>
#include <Scope/GeneralPurpose/FloatRingBuffer.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __BufferedFloatStreamPrivateData{
    IFloatStream parent;
    FloatRingBufferHandle buffer;
} BufferedFloatStreamPrivateData;

/******************************************************************************
 Public functions
******************************************************************************/
static bool dataIsReady(IFloatStreamHandle stream){
    BufferedFloatStreamHandle self = (BufferedFloatStreamHandle) stream->handle;

    if(FloatRingBuffer_getNumberOfUsedData(self->buffer) > 0){
        return true;
    }else{
        return false;
    }
}

static float readData(IFloatStreamHandle stream){
    BufferedFloatStreamHandle self = (BufferedFloatStreamHandle) stream->handle;

    float data;
    FloatRingBuffer_read(self->buffer, &data, 1);

    return data;
}

static void readAll(IFloatStreamHandle stream, float* data, const size_t length){
    BufferedFloatStreamHandle self = (BufferedFloatStreamHandle) stream->handle;

    FloatRingBuffer_read(self->buffer, data, length);
}

static size_t streamLength(IFloatStreamHandle stream){
    BufferedFloatStreamHandle self = (BufferedFloatStreamHandle) stream->handle;

    return FloatRingBuffer_getNumberOfUsedData(self->buffer);
}

static void writeData(IFloatStreamHandle stream, const float data){
    BufferedFloatStreamHandle self = (BufferedFloatStreamHandle) stream->handle;

    if(FloatRingBuffer_write(self->buffer, &data, 1) == -1){
        float dump;
        FloatRingBuffer_read(self->buffer, &dump, 1);
        FloatRingBuffer_write(self->buffer, &data, 1);
    }
}

static void writeAll(IFloatStreamHandle stream, const float* data, const size_t length){
    for(int i = 0; i < length; ++i){
        stream->writeData(stream, data[i]);
    }
}

static void flush(IFloatStreamHandle stream){
    BufferedFloatStreamHandle self = (BufferedFloatStreamHandle) stream->handle;

    FloatRingBuffer_clear(self->buffer);
}

BufferedFloatStreamHandle BufferedFloatStream_create(size_t capacity){

    BufferedFloatStreamHandle self = (BufferedFloatStreamHandle) malloc(sizeof(BufferedFloatStreamPrivateData));

    self->buffer = FloatRingBuffer_create(capacity);

    self->parent.handle = self;
    self->parent.dataIsReady = &dataIsReady;
    self->parent.readData = &readData;
    self->parent.length = &streamLength;
    self->parent.read = &readAll;
    self->parent.writeData = writeData;
    self->parent.write = writeAll;
    self->parent.flush = flush;

    return self;
}

void BufferedFloatStream_destroy(BufferedFloatStreamHandle self){
    FloatRingBuffer_destroy(self->buffer);

    free(self);
    self = NULL;
}

IFloatStreamHandle BufferedFloatStream_getIFloatStream(BufferedFloatStreamHandle self){
    return &self->parent;
}