/*!****************************************************************************************************************************************
 * @file         ICommunicator.h
 *
 * @copyright    Copyright (c) 2021 by Source Engineers GmbH. All Rights Reserved.
 *
 * @license {    This file is part of se-scope-target.
 *
 *               se-scope-target is free software; you can redistribute it and/or
 *               modify it under the terms of the GPLv3 General Public License Version 3
 *               as published by the Free Software Foundation.
 *
 *               se-scope-target is distributed in the hope that it will be useful,
 *               but WITHOUT ANY WARRANTY; without even the implied warranty of
 *               MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *               GNU General Public License for more details.
 *
 *               You should have received a copy of the GPLv3 General Public License Version 3
 *               along with se-scope-target.  If not, see <http://www.gnu.org/licenses/>.
 *
 *               In closed source or commercial projects, GPLv3 General Public License Version 3
 *               is not valid. In these cases the purchased commercial license is applied.
 *               Please contact us at scope@sourceengineers.com for a commercial license.
 * }
 *
 * @authors      Samuel Schuepbach <samuel.schuepbach@sourceengineers.com>
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
