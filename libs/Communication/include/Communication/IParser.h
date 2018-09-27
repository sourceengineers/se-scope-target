/*!****************************************************************************************************************************************
 * @file         IParser.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Parser interface.
 *
 *****************************************************************************************************************************************/

#ifndef IPARSER_H_
#define IPARSER_H_

#include <stdlib.h>

typedef struct IParserStruct* IParserHandle;

typedef struct IParserStruct {
  void* implementer;
  bool (*unpack)(IParserHandle iParserHandle, const char* data, const int length);
  
  const size_t (*getNumberOfCommands)(IParserHandle iParserHandle);
  bool (*getNameOfCommand)(IParserHandle self, char* name, const int maxLenght, const int index);
  
  
  /* Data fetcher functions */
  int (*getIntFromCommand)(IParserHandle iParserHandle,const char* commandName, const int offset);
  float (*getFloatFromCommand)(IParserHandle iParserHandle,const char* commandName, const int offset);
  bool (*getBoolFromCommand)(IParserHandle iParserHandle,const char* commandName, const int offset);
  void (*getStringFromCommand)(IParserHandle iParserHandle,const char* commandName, const int offset, char* targetStr, const int maxLenght);
} IParser ;

#endif
