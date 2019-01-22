/*!*****************************************************************************
 * @file         IRunnable.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief
 ******************************************************************************/

#ifndef IRUNNABLE_H_
#define IRUNNABLE_H_

#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct IRunnaleStruct* IRunnableHandle;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct IRunnableStruct {
    GenericReference handle;
    void (*run)(IRunnableHandle runnable);

} IRunnable ;

#endif
