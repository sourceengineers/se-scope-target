/*!*****************************************************************************
 * @file         IObserver.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Interface to implement the Observer design pattern
 *
 ******************************************************************************/

#ifndef IOBSERVER_H
#define IOBSERVER_H

#include "Scope/GeneralPurpose/DataTypes.h"

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct IObserverStruct* IObserverHandle;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct IObserverStruct{
    GenericReference handle;

    /**
     * Calls an update on the class which implemented the observer
     * @param runnable
     * @param state
     */
    void (* update)(IObserverHandle runnable, void* state);

} IObserver;
#endif
