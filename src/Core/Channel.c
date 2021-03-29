/*!****************************************************************************************************************************************
 * @file         Channel.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @license      This file is part of SE-Scope.
 *
 *               SE-Scope is free software; you can redistribute it and/or
 *               modify it under the terms of the GNU General Public License as
 *               published by the Free Software Foundation.
 *
 *               SE-Scope is distributed in the hope that it will be useful,
 *               but WITHOUT ANY WARRANTY; without even the implied warranty of
 *               MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *               GNU General Public License for more details.
 *
 *               You should have received a copy of the GNU General Public License
 *               along with SE-Scope.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/
#include <se-lib-c/stream/BufferedFloatStream.h>
#include <se-lib-c/container/FloatRingBuffer.h>

#include "Scope/GeneralPurpose/DataTypes.h"
#include "Scope/Core/Channel.h"
#include "Scope/Core/ScopeTypes.h"

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef enum {
    POLL_BUFFER, SWAP_BUFFER
} BUFFER_TYPE;

typedef struct __ChannelPrivateData {
    FloatRingBufferHandle buffers[2];
    IFloatStreamHandle stream;
    CHANNEL_STATES state;
    DATA_TYPES pollDataType;

    void *pollAddress;
    float oldTriggerData;

} ChannelPrivateData;

/* Casts the data of the address to float */
static float castDataToFloat(ChannelHandle self);

/* Pushes the latest data point into the trigger stream and copies the last one
 * further back */
static void addTriggerData(ChannelHandle self, float triggerData);

/* Sets a new state */
static void setState(ChannelHandle self, CHANNEL_STATES state);

/* Returns the state of the channel */
static CHANNEL_STATES getState(ChannelHandle self);

/******************************************************************************
 Private functions
******************************************************************************/
static float castDataToFloat(ChannelHandle self) {

    ADDRESS_DATA_TYPE transportAddr;

    float data;

    switch (self->pollDataType) {
        case SE_INT8:
            transportAddr = *((ADDRESS_DATA_TYPE *) self->pollAddress);
            data = ((int8_t) *((int8_t *) &transportAddr));
            break;
        case SE_INT16:
            transportAddr = *((ADDRESS_DATA_TYPE *) self->pollAddress);
            data = ((int16_t) *((int16_t *) &transportAddr));
            break;
        case SE_INT32:
            transportAddr = *((ADDRESS_DATA_TYPE *) self->pollAddress);
            data = ((int32_t) *((int32_t *) &transportAddr));
            break;
        case SE_UINT8:
            transportAddr = *((ADDRESS_DATA_TYPE *) self->pollAddress);
            data = ((uint8_t) *((uint8_t *) &transportAddr));
            break;
        case SE_UINT16:
            transportAddr = *((ADDRESS_DATA_TYPE *) self->pollAddress);
            data = ((uint16_t) *((uint16_t *) &transportAddr));
            break;
        case SE_UINT32:
            transportAddr = *((ADDRESS_DATA_TYPE *) self->pollAddress);
            data = ((uint32_t) *((uint32_t *) &transportAddr));
            break;
        case SE_FLOAT:
            transportAddr = *((ADDRESS_DATA_TYPE *) self->pollAddress);
            data = ((float) *((float *) &transportAddr));
            break;

        default:
            transportAddr = *((ADDRESS_DATA_TYPE *) self->pollAddress);
            data = ((float) *((float *) &transportAddr));
            break;
    }
    return data;
}

static void addTriggerData(ChannelHandle self, float triggerData) {

    self->stream->flush(self->stream);

    self->stream->writeData(self->stream, self->oldTriggerData);
    self->stream->writeData(self->stream, triggerData);

    self->oldTriggerData = triggerData;
}

static void setState(ChannelHandle self, CHANNEL_STATES state) {
    self->state = state;
}

static CHANNEL_STATES getState(ChannelHandle self) {
    return self->state;
}

/******************************************************************************
 Public functions
******************************************************************************/
ChannelHandle Channel_create(size_t capacity) {

    ChannelHandle self = malloc(sizeof(ChannelPrivateData));
    assert(self);
    self->stream = BufferedFloatStream_getIFloatStream(BufferedFloatStream_create(4));

    /* Set private variables */
    self->state = CHANNEL_INIT;
    self->buffers[POLL_BUFFER] = FloatRingBuffer_create(capacity);
    self->buffers[SWAP_BUFFER] = FloatRingBuffer_create(capacity);
    self->oldTriggerData = 0.0f;

    return self;
}

size_t Channel_getCapacity(ChannelHandle self) {
    return FloatRingBuffer_getCapacity(self->buffers[POLL_BUFFER]);
}

void Channel_destroy(ChannelHandle self) {
    FloatRingBuffer_destroy(self->buffers[POLL_BUFFER]);
    self->buffers[POLL_BUFFER] = NULL;
    FloatRingBuffer_destroy(self->buffers[SWAP_BUFFER]);
    self->buffers[SWAP_BUFFER] = NULL;
    free(self);
    self = NULL;
}

bool Channel_isRunning(ChannelHandle self) {
    if (self->state == CHANNEL_RUNNING) {
        return true;
    }
    return false;
}

void Channel_setPollAddress(ChannelHandle self, void *pollAddress, DATA_TYPES pollDataType) {
    self->pollAddress = pollAddress;
    self->pollDataType = pollDataType;
    if (getState(self) == CHANNEL_INIT) {
        setState(self, CHANNEL_STOPPED);
    }
}

void *Channel_getPollAddress(ChannelHandle self) {
    return self->pollAddress;
}

bool Channel_setStateRunning(ChannelHandle self) {
    if (getState(self) == CHANNEL_STOPPED) {
        setState(self, CHANNEL_RUNNING);
        return true;
    } else {
        return false;
    }
}

bool Channel_setStateStopped(ChannelHandle self) {
    if (getState(self) == CHANNEL_RUNNING) {
        setState(self, CHANNEL_STOPPED);
        return true;
    } else {
        return false;
    }
}

IFloatStreamHandle Channel_getTriggerDataStream(ChannelHandle self) {
    return self->stream;
}

void Channel_poll(ChannelHandle self) {
    if (getState(self) == CHANNEL_RUNNING) {
        const float polledData = castDataToFloat(self);
        addTriggerData(self, polledData);

        /* Channel start reading data out of the buffer if it is full */
        if (FloatRingBuffer_write(self->buffers[POLL_BUFFER], &polledData, 1) == -1) {
            float dump;
            FloatRingBuffer_read(self->buffers[POLL_BUFFER], &dump, 1);
            FloatRingBuffer_write(self->buffers[POLL_BUFFER], &polledData, 1);
        }
    } else {
        return;
    }
}

void Channel_clear(ChannelHandle self) {
    FloatRingBuffer_clear(self->buffers[POLL_BUFFER]);
    FloatRingBuffer_clear(self->buffers[SWAP_BUFFER]);
    self->stream->flush(self->stream);
}

int Channel_read(ChannelHandle self, float data[], size_t size) {
    return FloatRingBuffer_read(self->buffers[SWAP_BUFFER], data, size);
}

FloatRingBufferHandle Channel_getBuffer(ChannelHandle self) {
    return self->buffers[SWAP_BUFFER];
}

bool Channel_swapIsPending(ChannelHandle self) {

    bool pollBufferIsFull = FloatRingBuffer_getNumberOfFreeData(self->buffers[POLL_BUFFER]) == 0;
    bool swapBufferIsEmpty = FloatRingBuffer_getNumberOfUsedData(self->buffers[SWAP_BUFFER]) == 0;

    return pollBufferIsFull && swapBufferIsEmpty;
}

void Channel_swapBuffers(ChannelHandle self) {
    FloatRingBufferHandle buffer = self->buffers[SWAP_BUFFER];
    self->buffers[SWAP_BUFFER] = self->buffers[POLL_BUFFER];
    self->buffers[POLL_BUFFER] = buffer;
    FloatRingBuffer_clear(self->buffers[POLL_BUFFER]);
}

size_t Channel_getAmountOfUsedSwapData(ChannelHandle self) {
    return FloatRingBuffer_getNumberOfUsedData(self->buffers[SWAP_BUFFER]);
}

size_t Channel_getAmountOfUsedPollData(ChannelHandle self) {
    return FloatRingBuffer_getNumberOfUsedData(self->buffers[POLL_BUFFER]);
}
