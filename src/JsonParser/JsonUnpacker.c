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
  char* inputString;
  char* storageString;
  jsmntok_t* storageTokens;
} JsonUnpackerPrivateData ;

static int matchKeyToIndex(const char *json, jsmntok_t *tok, const char *key, int msgLength);

static bool jsoneq(const char *json, jsmntok_t *tok, const char *key);
/******************************************************************************
 Private functions
******************************************************************************/
static jsmntok_t getToken(const char *json, jsmntok_t *tok, const char *key, int msgLength){

  const int index = matchKeyToIndex(json, tok, key, msgLength);

  if(index < 0){
    jsmntok_t tok = {JSMN_UNDEFINED, 0, 0, 0};
    return tok;
  }

  return tok[index];
}

static jsmntok_t getCommandField(JsonUnpackerHandle self){
  jsmntok_t tok = getToken(self->storageString, self->storageTokens, "payload", self->msgLength);
  return getToken(self->storageString, self->storageTokens, "sc_cmd", self->msgLength);
}

static int matchKeyToIndex(const char *json, jsmntok_t *tok, const char *key, int msgLength){

  for (size_t i = 0; i < msgLength; ++i) {
    if(jsoneq(json, &tok[i], key) == true){
      return i;
    }
  }

  return -1;
}

static bool jsoneq(const char *json, jsmntok_t *tok, const char *key) {
	if (tok->type == JSMN_STRING && (int) strlen(key) == tok->end - tok->start &&
			strncmp(json + tok->start, key, tok->end - tok->start) == 0) {
		return true;
	}
	return false;
}

static bool unpack(IUnpackerHandle iUnpackHandler, const char* data, const size_t length){

  JsonUnpackerHandle self = (JsonUnpackerHandle) iUnpackHandler->implementer;

  if(jsmn_parse(&self->parser, data, strlen(data), self->inputTokens, (gemmi_uint) self->msgLength) < 0){
    return false;
  }

  if(self->inputTokens[0].type != JSMN_OBJECT){
    return false;
  }

  jsmntok_t tok = getToken(data, self->inputTokens, (const char*) "payload", self->msgLength);

  if(tok.type == JSMN_UNDEFINED){
    return false;
  }

  tok = getToken(data, self->inputTokens, (const char*) "sc_cmd", self->msgLength);

  if((tok.type == JSMN_UNDEFINED) || (tok.size <= 0)){
    return false;
  }

  return true;
}

static void activateNewMessage(IUnpackerHandle iUnpackHandler){
  JsonUnpackerHandle self = (JsonUnpackerHandle) iUnpackHandler->implementer;

  memcpy(self->storageTokens, self->inputTokens, self->msgLength);
  memcpy(self->storageString, self->inputString, strlen(self->inputString));

  jsmntok_t tok = getToken(self->storageString, self->storageTokens, (const char*) "sc_cmd", self->msgLength);

  self->numberOfCommands = (size_t) tok.size;
}

static const size_t getNumberOfCommands(IUnpackerHandle iUnpackHandler){
  JsonUnpackerHandle self = (JsonUnpackerHandle) iUnpackHandler->implementer;

  return self->numberOfCommands;
}

static bool getNameOfCommand(IUnpackerHandle iUnpackHandler, char* name, const int maxLenght, const int index){
  JsonUnpackerHandle self = (JsonUnpackerHandle) iUnpackHandler->implementer;

  jsmntok_t tok = getCommandField(self);

  if(tok.type != JSMN_OBJECT){
    return false;
  }

  if(tok.size <= index){
    return false;
  }

  jsmntok_t field = self->storageTokens[tok.start + index];

  if(field.type != JSMN_STRING){
    return false;
  }

  if(field.size > maxLenght){
    return false;
  }

  copyString(name, self->storageString + field.start, field.start - field.end);

  return true;
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
