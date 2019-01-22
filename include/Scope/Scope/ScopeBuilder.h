/*!*****************************************************************************
 * @file         ScopeBuilder.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Build the Scope
 *
 ******************************************************************************/

#ifndef SCOPEBUILDER_H
#define SCOPEBUILDER_H

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __ScopeBuilderPrivateData* ScopeBuilderHandle;

/******************************************************************************
 Public functions
******************************************************************************/
/* Constructor: Creates a new instance of the channel */
ScopeBuilderHandle ScopeBuilder_create(size_t capacity);

/* Deconstructor: Deletes the instance of the channel */
void ScopeBuilder_destroy(ScopeBuilderHandle self);



#endif SCOPEBUILDER_H
