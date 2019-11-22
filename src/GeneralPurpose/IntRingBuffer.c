/*!****************************************************************************************************************************************
 * @file         IntRingBuffer.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include "Scope/GeneralPurpose/IntRingBuffer.h"

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __IntRingBufferPrivateData{
    uint32_t* data;
    uint32_t* head;
    uint32_t* tail;
    size_t capacity;

} IntRingBufferPrivateData;

/* Returns the next index of the given index */
static uint32_t* nextIndex(IntRingBufferHandle self, uint32_t* index);

/* Increments the tail index */
static bool incTail(IntRingBufferHandle self);

/* Increments the head index */
static bool incHead(IntRingBufferHandle self);

/******************************************************************************
 Private functions
******************************************************************************/
static uint32_t* nextIndex(IntRingBufferHandle self, uint32_t* index){
    const uint32_t positionRelative = ((index + 1) - self->data);
    return (positionRelative % self->capacity) + self->data;
}

static bool incTail(IntRingBufferHandle self){
    if(self->tail != self->head){
        self->tail = nextIndex(self, self->tail);
        return true;
    }
    return false;
}

static bool incHead(IntRingBufferHandle self){
    if(nextIndex(self, self->head) != self->tail){
        self->head = nextIndex(self, self->head);
        return true;
    }
    return false;
}

/******************************************************************************
 Public functions
******************************************************************************/
IntRingBufferHandle IntRingBuffer_create(size_t capacity){

    /* Allocate memory and set _private variables */
    IntRingBufferHandle self = malloc(sizeof(IntRingBufferPrivateData));
    assert(self);
    self->capacity = capacity + 1;
    self->data = malloc(sizeof(uint32_t) * self->capacity);
    assert(self->data);

    self->tail = self->data;
    self->head = self->data;

    return self;
}

void IntRingBuffer_destroy(IntRingBufferHandle self){
    free(self->data);
    self->data = NULL;
    free(self);
    self = NULL;
}

size_t IntRingBuffer_getCapacity(IntRingBufferHandle self){
    return self->capacity - 1;
}

size_t IntRingBuffer_getNumberOfFreeData(IntRingBufferHandle self){
    return (size_t) (self->capacity - (IntRingBuffer_getNumberOfUsedData(self))) - 1;
}

size_t IntRingBuffer_getNumberOfUsedData(IntRingBufferHandle self){

    const size_t absSize = (self->head >= self->tail) ?
                           (self->head - self->tail) :
                           (self->capacity - (self->tail - self->head));

    return absSize % self->capacity;
}

void IntRingBuffer_clear(IntRingBufferHandle self){
    self->head = self->data;
    self->tail = self->data;
}

int IntRingBuffer_write(IntRingBufferHandle self, const uint32_t* data, const size_t length){

    if(length > IntRingBuffer_getNumberOfFreeData(self)){
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

int IntRingBuffer_readNoPosInc(IntRingBufferHandle self, uint32_t* data, const size_t length){

    uint32_t* tailBackup = self->tail;
    int ret = IntRingBuffer_read(self, data, length);
    self->tail = tailBackup;
		return ret;
}

int IntRingBuffer_read(IntRingBufferHandle self, uint32_t* data, const size_t length){

    if(length > IntRingBuffer_getNumberOfUsedData(self)){
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
