/*!*****************************************************************************
 * @file         ScopeUartStack.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @license This file is part of "se-scope-target". It is release under a commercial 
 *          license. Refer to the license PDF file received together with this code 
 *          for the exact licensing terms. 
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *
 * @brief        Builds a Uart Json stack.
 *
 ******************************************************************************/

#ifndef SCOPEUARTSTACK_H_
#define SCOPEUARTSTACK_H_

#include "Scope/Control/AnnounceStorage.h"
#include "Scope/GeneralPurpose/IMutex.h"
#include "Scope/Communication/ITransceiver.h"

/******************************************************************************
 Configuration interface for the ScopeUartStack
******************************************************************************/

/**
 * Configuration struct for the scope
 */
typedef struct __ScopeUartStackConfig {
    size_t sizeOfChannels;                  // Size of the channels
    size_t  amountOfChannels;
    TransmitCallback callback;          // Callback to the UsartTransmit method defined by the user
    uint32_t* timestamp;                    // Timestamp as base for the scope. This has to be incremented by the user.
    size_t  addressesInAddressAnnouncer;    // Amount of addresses that can be stored to be transmitted
    float timebase;                         // Timebase of the system
} ScopeUartStackConfig;

/**
 * If the scope is run in a mutli threaded context, two mutexes have to be provided to allow the scope to
 * protect itself from race conditions
 */
typedef struct __ScopeUartStackMutex {
    IMutexHandle configMutex;
    IMutexHandle dataMutex;
} ScopeUartStackMutex;

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __ScopeUartStackPrivateData* ScopeUartStackHandle;

/******************************************************************************
 Public functions
******************************************************************************/
/**
 *
 * @param config
 * @return
 */
ScopeUartStackHandle ScopeUartStack_create(ScopeUartStackConfig config);

/**
 * Constructor. Creates a static instance of the stack and scope. If the stack is created with this function, it can be used in a
 * multithreaded environment.
 * @param config
 * @param mutexes
 */
ScopeUartStackHandle ScopeUartStack_createThreadSafe(ScopeUartStackConfig config, ScopeUartStackMutex mutexes);

/**
 * Runs the JsonUart stack
 * @param self
 */
void ScopeUartStack_run(ScopeUartStackHandle self);

/**
 * Runs the scope in a thread safe environment.
 * @param self
 */
void ScopeUartStack_runThreadScope(ScopeUartStackHandle self);

/**
 * Runs the stack in a thread safe environment.
 * @param self
 */
void ScopeUartStack_runThreadStack(ScopeUartStackHandle self);

/**
 * Returns the handle to the announcement sotrage.
 * @return
 * @param self
 */
AnnounceStorageHandle ScopeUartStack_getAnnounceStorage(ScopeUartStackHandle self);

/**
 * Returns the transceiver interface
 * @return
 * @param self
 */
ITransceiverHandle ScopeUartStack_getTranscevier(ScopeUartStackHandle self);

/**
 * Deconstructor
 * @param self
 */
void ScopeUartStack_destroy(ScopeUartStackHandle self);
#endif
