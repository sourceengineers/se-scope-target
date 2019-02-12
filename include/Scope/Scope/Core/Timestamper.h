/*!*****************************************************************************
 * @file         Timestamper.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Impelments the channels, which will be attached to the scope
 * 
 ******************************************************************************/

#ifndef TIMESTAMPER_H_
#define TIMESTAMPER_H_

#include <Scope/GeneralPurpose/DataTypes.h>
#include <Scope/GeneralPurpose/IIntStream.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __TimestamperPrivateData* TimestamperHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the channel */
TimestamperHandle Timestamper_create(size_t capacity, uint32_t* referenceTimestamp);

/* Deconstructor: Deletes the instance of the channel */
void Timestamper_destroy(TimestamperHandle self);

bool Timestamper_updateElapsedTime(TimestamperHandle self);

void Timestamper_stamp(TimestamperHandle self);

void Timestamper_setStateRunning(TimestamperHandle self);

void Timestamper_setStateStopped(TimestamperHandle self);

uint32_t Timestamper_getCurrentTime(TimestamperHandle self);

uint32_t Timerstamper_getTimeIncrement(TimestamperHandle self);

void Timestamper_configureTimestampIncrement(TimestamperHandle self, uint32_t timstampIncrement);

void Timestamper_clear(TimestamperHandle self);

IIntStreamHandle Timestamper_getStream(TimestamperHandle self);

#endif
