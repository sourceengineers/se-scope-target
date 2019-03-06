/*!*****************************************************************************
 * @file         IObserver.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 ******************************************************************************/

#ifndef IOBSERVER_H
#define IOBSERVER_H

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct IObserverStruct* IObserverHandle;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct IObserverStruct{
    GenericReference handle;

    void (* update)(IObserverHandle runnable, void* state);

} IObserver;
#endif