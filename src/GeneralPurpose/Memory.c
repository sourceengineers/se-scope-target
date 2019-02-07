/*!****************************************************************************************************************************************
 * @file         Memory.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/GeneralPurpose/Memory.h>
#include <string.h>

void copyString(char *str, const char *data, size_t size){
  memcpy(str, data, size);
  str[size] = '\0';
}