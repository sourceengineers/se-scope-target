/*!*****************************************************************************
 * @file         ScopeRunner.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Run the runnables in ScopeObject
 *
 ******************************************************************************/

#ifndef SCOPERUNNER_H
#define SCOPERUNNER_H

#include <Scope/Builders/ScopeBuilder.h>

/******************************************************************************
 Public functions
******************************************************************************/
/**
 * Runs the scope
 * @param scope Object filled with runnables, generated by the ScopeBuilder
 */
void ScopeRunner_run(ScopeObject scope);

#endif
