/*!****************************************************************************************************************************************
 * @file         ICommand.h
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
 *               is not valid. In this case the commercial license received with the purchase
 *               is applied (See SeScopeLicense.pdf).
 *               Please contact us at scope@sourceengineers.com for a commercial license.
 * }
 *
 * @authors      Samuel Schuepbach <samuel.schuepbach@sourceengineers.com>
 *
 * @brief        Streaming interface
 *               run: Runs the command
 *               setCommandAttribute: Sets the attributes needed by the command
 *               to run. The data passed to the void pointer, is specified by 
 *               every command on its own.
 *               getCommandName: returns the name of the command
 *
 *****************************************************************************************************************************************/

#ifndef ICOMMAND_H_
#define ICOMMAND_H_

#include "Scope/GeneralPurpose/DataTypes.h"

#include <stdlib.h>

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct ICommandStruct* ICommandHandle;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct ICommandStruct{
    SeScopeGenericReference handle;

    /**
     * Runnable
     * @param command
     */
    void (* run)(ICommandHandle command);
} ICommand;

#endif
