/*!*****************************************************************************
 * @file         ObserverMock.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach <samuel.schuepbach@sourceengineers.com>
 *
 * @brief        Mock of a observer implementation to be used for testing
 *
 ******************************************************************************/

#ifndef OBSERVERMOCK_H
#define OBSERVERMOCK_H

#include <stdbool.h>
#include <stdint.h>
#include <se-lib-c/util/observer/IObserver.h>

typedef struct __ObserverMockPrivateData {

    IObserver observer;
    bool updateHasBeenCalled;
    uint32_t updateCalledWidth;

} ObserverMockPrivateData;

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __ObserverMockPrivateData* ObserverMockHandle;

/******************************************************************************
 Public functions
******************************************************************************/
/**
 * Constructor
 * @param scope
 * @param packer
 * @param unpacker
 * @param announceStorage
 * @return
 */
ObserverMockHandle ObserverMock_create();

/**
 *
 * @param self
 * @return
 */
IObserverHandle ObserverMock_getIObserver(ObserverMockHandle self);

/**
 * Deconstructor
 * @param self
 */
void ObserverMock_destroy(ObserverMockHandle self);

#endif


