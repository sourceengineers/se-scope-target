/*!*****************************************************************************
 * @file         ObserverMock.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Mock of a observer implementation to be used for testing
 *
 ******************************************************************************/

#include "ObserverMock.h"
#include <stdlib.h>

static void update(IObserverHandle observer, void* state) {
    ObserverMockHandle self = (ObserverMockHandle) observer->handle;

    self->updateCalledWidth = *(uint32_t*) state;
    self->updateHasBeenCalled = true;
}

ObserverMockHandle ObserverMock_create(){

    ObserverMockHandle self = malloc(sizeof(ObserverMockPrivateData));

    self->observer.handle = self;
    self->observer.update = &update;

    self->updateHasBeenCalled = false;
    self->updateCalledWidth = 0;

    return self;
}

IObserverHandle ObserverMock_getIObserver(ObserverMockHandle self){
    return &self->observer;
}


void ObserverMock_destroy(ObserverMockHandle self){
    free(self);
    self = NULL;
}