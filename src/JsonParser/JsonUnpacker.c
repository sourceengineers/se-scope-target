/*!****************************************************************************************************************************************
 * @file         JsonUnpacker.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/JsonParser/JsonUnpacker.h>
#include <string.h>
#include <Scope/GeneralPurpose/Memory.h>
#include <Scope/GeneralPurpose/IByteStream.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __JsonUnpackerPrivateData
{

  IUnpacker iUnpacker;

  size_t numberOfCommands;
  size_t msgLength;

  jsmn_parser parser;
  jsmntok_t* inputTokens;
  jsmntok_t* storageTokens;
} JsonUnpackerPrivateData ;
/******************************************************************************
 Private functions
******************************************************************************/
static bool unpack(IUnpackerHandle iUnpackHandler, const char* data, const size_t length){

  JsonUnpackerHandle self = (JsonUnpackerHandle) iUnpackHandler->implementer;

  if(jsmn_parse(&self->parser, data, strlen(data), self->inputTokens, (gemmi_uint) self->msgLength) < 0){
    return false;
  }
}

static void activateNewMessage(IUnpackerHandle iUnpackHandler){
  JsonUnpackerHandle self = (JsonUnpackerHandle) iUnpackHandler->implementer;

  memcpy(self->storageTokens, self->inputTokens, self->msgLength);
}

static const size_t getNumberOfCommands(IUnpackerHandle iUnpackHandler){
  JsonUnpackerHandle self = (JsonUnpackerHandle) iUnpackHandler->implementer;

}

static bool getNameOfCommand(IUnpackerHandle iUnpackHandler, char* name, const int maxLenght, const int index){
  JsonUnpackerHandle self = (JsonUnpackerHandle) iUnpackHandler->implementer;
}

static gemmi_uint getIntFromCommand(IUnpackerHandle iUnpackHandler, CommandFetchingInformation* information){
  JsonUnpackerHandle self = (JsonUnpackerHandle) iUnpackHandler->implementer;
}

static float getFloatFromCommand(IUnpackerHandle iUnpackHandler, CommandFetchingInformation* information){
  JsonUnpackerHandle self = (JsonUnpackerHandle) iUnpackHandler->implementer;
}

static bool getBoolFromCommand(IUnpackerHandle iUnpackHandler, CommandFetchingInformation* information){
  JsonUnpackerHandle self = (JsonUnpackerHandle) iUnpackHandler->implementer;
}

static void getStringFromCommand(IUnpackerHandle iUnpackHandler, CommandFetchingInformation* information,
                                 char* targetStr,
                                 const int maxLenght){

  JsonUnpackerHandle self = (JsonUnpackerHandle) iUnpackHandler->implementer;
}

static bool getNameOfField(IUnpackerHandle iUnpackHandler, const char* commandName, char* fieldName,
                           const int maxLenght,
                           const int index){
  JsonUnpackerHandle self = (JsonUnpackerHandle) iUnpackHandler->implementer;
}

static size_t getLengthOfCheck(IUnpackerHandle iUnpackHandler){
  JsonUnpackerHandle self = (JsonUnpackerHandle) iUnpackHandler->implementer;
}

static size_t getLengthOfBytesToCheck(IUnpackerHandle iUnpackHandler){
  JsonUnpackerHandle self = (JsonUnpackerHandle) iUnpackHandler->implementer;
}

static void getBytesToCheck(IUnpackerHandle iUnpackHandler, uint8_t* data){
  JsonUnpackerHandle self = (JsonUnpackerHandle) iUnpackHandler->implementer;
}

static void getCheck(IUnpackerHandle iUnpackHandler, uint8_t* checkData){
  JsonUnpackerHandle self = (JsonUnpackerHandle) iUnpackHandler->implementer;
}

static ssize_t getNumberOfFields(IUnpackerHandle iUnpackHandler, const char* commandName){
  JsonUnpackerHandle self = (JsonUnpackerHandle) iUnpackHandler->implementer;
}

/******************************************************************************
 Public functions
******************************************************************************/
JsonUnpackerHandle JsonUnpacker_create(const size_t msgLength){
  
  JsonUnpackerHandle self = (JsonUnpackerHandle) malloc(sizeof(JsonUnpackerPrivateData));

  self->numberOfCommands = 0;
  self->msgLength = msgLength;
  self->iUnpacker.implementer = self;
  self->iUnpacker.unpack = &unpack;
  self->iUnpacker.getBoolFromCommand = &getBoolFromCommand;
  self->iUnpacker.getFloatFromCommand = &getFloatFromCommand;
  self->iUnpacker.getIntFromCommand = &getIntFromCommand;
  self->iUnpacker.getStringFromCommand = &getStringFromCommand;
  self->iUnpacker.getNameOfCommand = &getNameOfCommand;
  self->iUnpacker.getNumberOfCommands = &getNumberOfCommands;
  self->iUnpacker.getNumberOfFields = &getNumberOfFields;
  self->iUnpacker.getNameOfField = &getNameOfField;
  self->iUnpacker.activateNewMessage = &activateNewMessage;
  self->iUnpacker.getLengthOfBytesToCheck = &getLengthOfBytesToCheck;
  self->iUnpacker.getLengthOfCheck = &getLengthOfCheck;
  self->iUnpacker.getCheck = &getCheck;
  self->iUnpacker.getBytesToCheck = &getBytesToCheck;

  self->inputTokens = (jsmntok_t*) malloc(sizeof(jsmntok_t) * self->msgLength);
  self->storageTokens = (jsmntok_t*) malloc(sizeof(jsmntok_t) * self->msgLength);

  jsmn_init(&self->parser);

  return self;
}

void JsonUnpacker_destroy(JsonUnpackerHandle self){
  free(self);
  self = NULL;
}

IUnpackerHandle JsonUnpacker_getIUnpacker(JsonUnpackerHandle self){
  return &self->iUnpacker;
}
