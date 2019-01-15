/*!****************************************************************************************************************************************
 * @file         BufferedByteStream.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/GeneralPurpose/BufferedByteStream.h>
#include <Scope/GeneralPurpose/ByteRingBuffer.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __BufferedByteStreamPrivateData {
    IByteStream parent;
    ByteRingBufferHandle buffer;
} BufferedByteStreamPrivateData;

/******************************************************************************
 Public functions
******************************************************************************/
static bool dataIsReady(IByteStreamHandle parent) {
    BufferedByteStreamHandle self = (BufferedByteStreamHandle) parent->implementer;

    if (ByteRingBuffer_getNumberOfUsedData(self->buffer) > 0) {
        return true;
    } else {
        return false;
    }
}

static uint8_t readData(IByteStreamHandle parent) {
    BufferedByteStreamHandle self = (BufferedByteStreamHandle) parent->implementer;

    uint8_t data;
    ByteRingBuffer_read(self->buffer, &data, 1);

    return data;
}

static void readAll(IByteStreamHandle parent, uint8_t *data, const size_t length) {
    BufferedByteStreamHandle self = (BufferedByteStreamHandle) parent->implementer;

    ByteRingBuffer_read(self->buffer, data, length);
}

static size_t streamLength(IByteStreamHandle parent) {
    BufferedByteStreamHandle self = (BufferedByteStreamHandle) parent->implementer;

    return ByteRingBuffer_getNumberOfUsedData(self->buffer);
}

static void writeData(IByteStreamHandle parent, const uint8_t data) {
    BufferedByteStreamHandle self = (BufferedByteStreamHandle) parent->implementer;

    ByteRingBuffer_write(self->buffer, &data, 1);
}

static void writeAll(IByteStreamHandle parent, const uint8_t *data, const size_t length) {
    BufferedByteStreamHandle self = (BufferedByteStreamHandle) parent->implementer;

    ByteRingBuffer_write(self->buffer, data, length);
}

static void flush(IByteStreamHandle parent) {
    BufferedByteStreamHandle self = (BufferedByteStreamHandle) parent->implementer;

    ByteRingBuffer_clear(self->buffer);
}

BufferedByteStreamHandle BufferedByteStream_create(size_t capacity) {

    BufferedByteStreamHandle self = (BufferedByteStreamHandle) malloc(sizeof(BufferedByteStreamPrivateData));

    self->buffer = ByteRingBuffer_create(capacity);

    self->parent.implementer = self;
    self->parent.byteIsReady = &dataIsReady;
    self->parent.readByte = &readData;
    self->parent.length = &streamLength;
    self->parent.read = &readAll;
    self->parent.writeByte = writeData;
    self->parent.write = writeAll;
    self->parent.flush = flush;

    return self;
}

void BufferedByteStream_destroy(BufferedByteStreamHandle self) {
    ByteRingBuffer_destroy(self->buffer);

    free(self);
    self = NULL;
}

IByteStreamHandle BufferedByteStream_getIByteStream(BufferedByteStreamHandle self){
    return &self->parent;
}