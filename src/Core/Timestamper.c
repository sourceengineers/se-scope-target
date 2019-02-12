/*!****************************************************************************************************************************************
 * @file         Timestamper.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Core/Timestamper.h>
#include <Scope/GeneralPurpose/DataTypes.h>
#include <Scope/GeneralPurpose/BufferedFloatStream.h>
#include <Scope/GeneralPurpose/BufferedIntStream.h>
#include <stdlib.h>

/******************************************************************************
 Define private data
******************************************************************************/
typedef enum{
    TIMESTAMPER_STOPPED, TIMESTAMPER_RUNNING
} TIMESTAMPER_STATES;

/* Class data */
typedef struct __TimestamperPrivateData{
    TIMESTAMPER_STATES state;

    /* Timestamping data */
    uint32_t timeIncrement;
    BufferedIntStreamHandle timestamp;
    uint32_t* referenceTimestamp;
    uint32_t lastTimestamp;

} TimestamperPrivateData;

/* Sets a new state */
static void setState(TimestamperHandle self, TIMESTAMPER_STATES state);

/* Returns the state of the channel */
static TIMESTAMPER_STATES getState(TimestamperHandle self);

/******************************************************************************
 Private functions
******************************************************************************/
static void setState(TimestamperHandle self, TIMESTAMPER_STATES state){
    self->state = state;
}

static TIMESTAMPER_STATES getState(TimestamperHandle self){
    return self->state;
}

/******************************************************************************
 Public functions
******************************************************************************/
TimestamperHandle Timestamper_create(size_t capacity, uint32_t* referenceTimestamp){

    TimestamperHandle self = malloc(sizeof(TimestamperPrivateData));
    self->timeIncrement = 1;
    self->referenceTimestamp = referenceTimestamp;
    self->lastTimestamp = 0;

    self->timestamp = BufferedIntStream_create(capacity);

    Timestamper_setStateStopped(self);

    return self;
}

void Timestamper_destroy(TimestamperHandle self){
    BufferedIntStream_destroy(self->timestamp);
    free(self);
    self = NULL;
}

bool Timestamper_updateElapsedTime(TimestamperHandle self){

    if(*self->referenceTimestamp < (self->lastTimestamp + self->timeIncrement)){
        return false;
    }
    return true;
}

void Timestamper_stamp(TimestamperHandle self){

    self->lastTimestamp = *self->referenceTimestamp;

    if(getState(self) == TIMESTAMPER_STOPPED){
        return;
    }

    IIntStreamHandle stream = BufferedIntStream_getIIntStream(self->timestamp);
    stream->writeData(stream, self->lastTimestamp);
}

void Timestamper_setStateRunning(TimestamperHandle self){
    setState(self, TIMESTAMPER_RUNNING);
}

uint32_t Timestamper_getCurrentTime(TimestamperHandle self){
    return self->lastTimestamp;
};

void Timestamper_setStateStopped(TimestamperHandle self){
    setState(self, TIMESTAMPER_STOPPED);
}

uint32_t Timerstamper_getTimeIncrement(TimestamperHandle self){
    return self->timeIncrement;
}

void Timestamper_configureTimestampIncrement(TimestamperHandle self, uint32_t timstampIncrement){
    self->timeIncrement = timstampIncrement;
}

void Timestamper_clear(TimestamperHandle self){
    IIntStreamHandle stream = BufferedIntStream_getIIntStream(self->timestamp);
    stream->flush(stream);
}

IIntStreamHandle Timestamper_getStream(TimestamperHandle self){
    return BufferedIntStream_getIIntStream(self->timestamp);
}
