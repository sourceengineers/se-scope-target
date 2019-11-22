/*!****************************************************************************************************************************************
 * @file         FloatRingBuffer.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include "Scope/GeneralPurpose/FloatRingBuffer.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __FloatRingBufferPrivateData{
    float* data;
    float* head;
    float* tail;
    size_t capacity;

} FloatRingBufferPrivateData;

/* Returns the next index of the given index */
static float* nextIndex(FloatRingBufferHandle self, float* index);

/* Increments the tail index */
static bool incTail(FloatRingBufferHandle self);

/* Increments the head index */
static bool incHead(FloatRingBufferHandle self);

/******************************************************************************
 Private functions
******************************************************************************/
static float* nextIndex(FloatRingBufferHandle self, float* index){
    const uint32_t positionRelative = ((index + 1) - self->data);
    return (positionRelative % self->capacity) + self->data;
}

static bool incTail(FloatRingBufferHandle self){
    if(self->tail != self->head){
        self->tail = nextIndex(self, self->tail);
        return true;
    }
    return false;
}

static bool incHead(FloatRingBufferHandle self){
    if(nextIndex(self, self->head) != self->tail){
        self->head = nextIndex(self, self->head);
        return true;
    }
    return false;
}

/******************************************************************************
 Public functions
******************************************************************************/
FloatRingBufferHandle FloatRingBuffer_create(size_t capacity){

    /* Allocate memory and set _private variables */
    FloatRingBufferHandle self = malloc(sizeof(FloatRingBufferPrivateData));
    assert(self);
    self->capacity = capacity + 1;
    self->data = malloc(sizeof(float) * self->capacity);
    assert(self->data);

    self->tail = self->data;
    self->head = self->data;

    return self;
}

void FloatRingBuffer_destroy(FloatRingBufferHandle self){
    free(self->data);
    self->data = NULL;
    free(self);
    self = NULL;
}

size_t FloatRingBuffer_getCapacity(FloatRingBufferHandle self){
    return self->capacity - 1;
}

size_t FloatRingBuffer_getNumberOfFreeData(FloatRingBufferHandle self){
    return (size_t) (self->capacity - (FloatRingBuffer_getNumberOfUsedData(self))) - 1;
}

size_t FloatRingBuffer_getNumberOfUsedData(FloatRingBufferHandle self){
    const size_t absSize = (self->head >= self->tail) ?
                           (self->head - self->tail) :
                           (self->capacity - (self->tail - self->head));

    return absSize % self->capacity;
}

void FloatRingBuffer_clear(FloatRingBufferHandle self){
    self->head = self->data;
    self->tail = self->data;
}

int FloatRingBuffer_write(FloatRingBufferHandle self, const float* data, const size_t length){

    if(length > FloatRingBuffer_getNumberOfFreeData(self)){
        return -1;
    }

    size_t i = 0;
    do{
        *(self->head) = data[i++];
        /* Catch buffer overflow */
        if(incHead(self) == false){
            return -1;
        }
    }while(length > i);

    return i;
}

int FloatRingBuffer_readNoPosInc(FloatRingBufferHandle self, float* data, const size_t length){

    float* tailBackup = self->tail;
    int ret = FloatRingBuffer_read(self, data, length);
    self->tail = tailBackup;
		return ret;
}

int FloatRingBuffer_read(FloatRingBufferHandle self, float* data, const size_t length){

    if(length > FloatRingBuffer_getNumberOfUsedData(self)){
        return -1;
    }

    size_t i = 0;
    do{
        data[i++] = *(self->tail);
        /* Catch buffer overflow */
        if(incTail(self) == false){
            return -1;
        }
    }while(length > i);

    return i;
}
