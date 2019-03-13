/*!****************************************************************************************************************************************
 * @file         ICommunicator.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Interface, which will have to be implemented by every communication type to present functions
 *               to validate that the communication was successful.
 *
 *
 *
 *****************************************************************************************************************************************/

#ifndef ICOMMUNICATOR_H_
#define ICOMMUNICATOR_H_

#include <Scope/GeneralPurpose/DataTypes.h>
#include <Scope/GeneralPurpose/IRunnable.h>
#include <Scope/GeneralPurpose/IObserver.h>

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct ICommunicatorStruct* ICommunicatorHandle;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct ICommunicatorStruct{
    GenericReference handle;

    /**
     * Returns the observer which is used to update the Tx path
     * @param communicator
     * @return
     */
    IObserverHandle (* getObserver)(ICommunicatorHandle communicator);

    /**
     * Attaches the observer which will update the Rx path
     * @param communicator
     * @param observer
     */
    void (* attachObserver)(ICommunicatorHandle communicator, IObserverHandle observer);

    /**
     * Returns the runnable of the tx path of the communication interface
     * @param communicator
     * @return
     */
    IRunnableHandle (* getTxRunnable)(ICommunicatorHandle communicator);

    /**
     * Returns the runnable of the rx path of the communication interface
     * @param communicator
     * @return
     */
    IRunnableHandle (* getRxRunnable)(ICommunicatorHandle communicator);

} ICommunicator;

#endif
