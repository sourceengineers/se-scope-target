/*!*****************************************************************************
 * @file         ScopeThreadRunner.h
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
 * @brief        Run the runnables in ScopeRunnable
 *
 ******************************************************************************/

#ifndef SCOPETHREADRUNNER_H
#define SCOPETHREADRUNNER_H

#include <se-lib-c/osal/IMutex.h>
#include "Scope/Builders/ScopeBuilder.h"

/******************************************************************************
 Public functions
******************************************************************************/
/**
 * Runs the scope in a thread context
 * @param scope Object filled with runnables, generated by the ScopeBuilder
 */
void ScopeThreadRunner_runScope(ScopeRunnable scope);

/**
 * Runs the stack in a thread context
 * @param scope Object filled with runnables, generated by the ScopeBuilder
 */
void ScopeThreadRunner_runStack(ScopeRunnable scope);

#endif
