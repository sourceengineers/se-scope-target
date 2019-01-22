/*!*****************************************************************************
 * @file         IRxTxRunnable.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief
 ******************************************************************************/

#ifndef IRXTXRUNNABLE_H_
#define IRXTXRUNNABLE_H_

#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct IRxTxRunnableStruct* IRxTxRunnableHandle;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct IRxTxRunnableStruct {
    GenericReference handle;
    void (*runRx)(IRxTxRunnableHandle runnable);
    void (*runTx)(IRxTxRunnableHandle runnable);

} IRxTxRunnable ;

#endif
