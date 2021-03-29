/*!****************************************************************************************************************************************
 * @file         ICommunicator.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @license      This file is part of SE-Scope.
 *
 *               SE-Scope is free software; you can redistribute it and/or
 *               modify it under the terms of the GNU General Public License as
 *               published by the Free Software Foundation.
 *
 *               SE-Scope is distributed in the hope that it will be useful,
 *               but WITHOUT ANY WARRANTY; without even the implied warranty of
 *               MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *               GNU General Public License for more details.
 *
 *               You should have received a copy of the GNU General Public License
 *               along with SE-Scope.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Interface, which will have to be implemented by every communication type to present functions
 *               to validate that the communication was successful.
 *
 *****************************************************************************************************************************************/

#ifndef ICOMMUNICATOR_H_
#define ICOMMUNICATOR_H_

#include "Scope/GeneralPurpose/DataTypes.h"
#include <se-lib-c/util/runnable/IRunnable.h>

#include <se-lib-c/util/observer/IObserver.h>

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct ICommunicatorStruct* ICommunicatorHandle;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct ICommunicatorStruct{
    SeScopeGenericReference handle;

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
