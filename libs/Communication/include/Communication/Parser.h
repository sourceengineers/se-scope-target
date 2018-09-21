/*!*****************************************************************************
 * @file         Parser.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the Scope.
 * 
 ******************************************************************************/
 
#ifndef PARSER_H_
#define PARSER_H_

#include <unistd.h>

/* Defines class */
typedef struct __ParserPrivateData* ParserHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instanze of the channel */
ParserHandle Parser_create();

/* Deconstructor: Deletes the instanze of the channel */
void Parser_destroy(ParserHandle self);


#endif
