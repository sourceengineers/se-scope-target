/*!****************************************************************************************************************************************
 * @file         Memory.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <GeneralPurpose/Memory.h>

void copyMemory(char* dest, const char* src, size_t length){
  for (size_t i = 0; i < length; ++i) {
    dest[i] = src[i];
  }
}

void copyString(char *str, const char *data, size_t size){
  copyMemory(str, data, size);
  str[size] = '\0';
}