/*!*****************************************************************************
 * @file         IRunnable.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Interface to be implemented if a runnable should be used
 *
 ******************************************************************************/

#ifndef IRUNNABLE_H_
#define IRUNNABLE_H_

#include "Scope/GeneralPurpose/DataTypes.h"

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct IRunnableStruct* IRunnableHandle;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct IRunnableStruct{
    GenericReference handle;

    /**
     * Runs the class which implemented the runnable
     * @param runnable
     */
    bool (* run)(IRunnableHandle runnable);

} IRunnable;

#endif
