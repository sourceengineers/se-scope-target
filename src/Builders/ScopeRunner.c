/*!****************************************************************************************************************************************
 * @file         ScopeRunner.c
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
 *****************************************************************************************************************************************/

#include "Scope/Builders/ScopeRunner.h"
#include "Scope/Control/Controller.h"
#include "Scope/Serialisation/Serializer.h"

/******************************************************************************
 Public functions
******************************************************************************/
void ScopeRunner_run(ScopeRunnable scope){

    scope.runCommunicationRx->run(scope.runCommunicationRx);
    scope.runUnpacker->run(scope.runUnpacker);
    scope.runCommandParser->run(scope.runCommandParser);
    scope.runScope->run(scope.runScope);
    scope.runDataAggregator->run(scope.runDataAggregator);
    scope.runPacker->run(scope.runPacker);
    scope.runCommunicationTx->run(scope.runCommunicationTx);
}
