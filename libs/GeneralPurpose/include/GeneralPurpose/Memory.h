/*!*****************************************************************************
 * @file         Memory.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementations of some basic memory functions
 *
 ******************************************************************************/
 
#ifndef MEMORY_H_
#define MEMORY_H_

#include <stddef.h>

void copyMemory(char* dest, const char* src, size_t length);

#endif