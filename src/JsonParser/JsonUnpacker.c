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

/* The function matches the end of a field to a start of another one. With this, fields at a specific index can
 * be found */

static jsmntok_t* getElementInObject(JsonUnpackerHandle self, jsmntok_t *tok, size_t index){

  gemmi_uint startPosition = tok->start;
  gemmi_uint foundNextTokens = 0;

  for (int i = 0; i < self->msgLength - 1; ++i) {

      if(self->storageTokens[i].start == startPosition){

        if(index == foundNextTokens){
          return &self->storageTokens[i];
        }

        foundNextTokens++;

        /* The next field starts with one character shifted more, if its a string field,
         * because of the used " of the string. */
        size_t positionOffset = (&self->storageTokens[i]+1)->type == JSMN_STRING ? 3 : 2;

        startPosition = (&self->storageTokens[i]+1)->end + positionOffset;
      }
  }

  return NULL;
}

static jsmntok_t* getToken(const char *json, jsmntok_t *tok, const char *key, int msgLength){

  const int index = matchKeyToIndex(json, tok, key, msgLength);

  if(index < 0){
    return NULL;
  }

  return &tok[index];
}

static jsmntok_t* getCommandField(JsonUnpackerHandle self){
  jsmntok_t* tok = getToken(self->storageString, self->storageTokens, (const char*) "sc_cmd", self->msgLength);

  return tok+1;
}


static jsmntok_t* getValueFromArray(JsonUnpackerHandle self, jsmntok_t* array, size_t index){

  if(array == NULL){
    return NULL;
  }
  if(array->type != JSMN_ARRAY){
    return NULL;
  }

  if(array->size < index){
    return NULL;
  }

  return array + 1 + index;
}

static jsmntok_t* getField(JsonUnpackerHandle self, jsmntok_t* command, const char* fieldName){

  /* Get the field instead of the command name */
  jsmntok_t* field = command + 1;

  if(field == NULL){
    return NULL;
  }
  if(field->type == JSMN_UNDEFINED){
    return NULL;
  }

  /* This token already might represent a value. If so, return said token. */
  if(field->type == JSMN_STRING || field->type == JSMN_PRIMITIVE){
    return field;
  }

  /* If not, it should be a object */
  if(field->type != JSMN_OBJECT){
    return NULL;
  }

  size_t numberOfFields = field->size;

  for (int i = 0; i < numberOfFields; ++i) {
    jsmntok_t* field = getElementInObject(self, command+2, i);

    if(field == NULL){
      return NULL;
    }

    char name[30];
    copyString(name, self->storageString + field->start, field->end - field->start);

    if(strncmp(name, fieldName, 30) == 0){
      return field+1;
    }
  }

  return NULL;
}

static jsmntok_t* getCommand(JsonUnpackerHandle self, const char* commandName){

  jsmntok_t* tok = getCommandField(self);

  if(tok == NULL){
    return false;
  }

  if(tok->type != JSMN_OBJECT){
    return false;
  }

  for (int i = 0; i < self->numberOfCommands; ++i) {

    jsmntok_t* command = getElementInObject(self, tok+1, i);

    if(command == NULL || command->type != JSMN_STRING){
      return NULL;
    }

    char name[30];
    copyString(name, self->storageString + command->start, command->end - command->start);

    if(strncmp(name, commandName, 30) == 0){
      return command;
    }

  }
  return NULL;
}

static int matchKeyToIndex(const char *json, jsmntok_t *tok, const char *key, int msgLength){

  for (int i = 0; i < msgLength; ++i) {
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

  jsmn_init(&self->parser);

  if(jsmn_parse(&self->parser, data, strlen(data), self->inputTokens, self->msgLength) < 0){
    return false;
  }

  if(self->inputTokens[0].type != JSMN_OBJECT){
    return false;
  }

  jsmntok_t* tok = getToken(data, self->inputTokens, (const char*) "payload", self->msgLength);

  if(tok == NULL){
    return false;
  }

  tok = getToken(data, self->inputTokens, (const char*) "sc_cmd", self->msgLength);

  if(tok == NULL){
    return false;
  }

  if((tok + 1)->type != JSMN_OBJECT){
    return false;
  }

  if((tok + 1)->size <= 0){
    return false;
  }

  strcpy(self->inputString, data);
  return true;
}

static void activateNewMessage(IUnpackerHandle iUnpackHandler){
  JsonUnpackerHandle self = (JsonUnpackerHandle) iUnpackHandler->implementer;

  memcpy(self->storageTokens, self->inputTokens, self->msgLength * sizeof(jsmntok_t));
  strcpy(self->storageString, self->inputString);

  jsmntok_t* tok = getCommandField(self);

  self->numberOfCommands = (size_t) tok->size;
}

static size_t getNumberOfCommands(IUnpackerHandle iUnpackHandler){
  JsonUnpackerHandle self = (JsonUnpackerHandle) iUnpackHandler->implementer;

  return self->numberOfCommands;
}

static bool getNameOfCommand(IUnpackerHandle iUnpackHandler, char* name, const int maxLenght, const int index){
  JsonUnpackerHandle self = (JsonUnpackerHandle) iUnpackHandler->implementer;

  jsmntok_t* tok = getCommandField(self);

  if(tok == NULL){
    return false;
  }

  if(tok->type != JSMN_OBJECT){
    return false;
  }

  if(tok->size <= index){
    return false;
  }

  jsmntok_t* field = getElementInObject(self, tok+1, index);

  if(field == NULL){
    return false;
  }

  if(field->type != JSMN_STRING){
    return false;
  }

  if(field->size > maxLenght){
    return false;
  }

  copyString(name, self->storageString + field->start, field->end - field->start);

  return true;
}

static gemmi_uint getIntFromCommand(IUnpackerHandle iUnpackHandler, CommandFetchingInformation* information){
  JsonUnpackerHandle self = (JsonUnpackerHandle) iUnpackHandler->implementer;

  jsmntok_t* field = getCommand(self, information->commandName);
  field = getField(self, field, information->fieldName);

  if(field == NULL){
    return 0;
  }

  /* If the field is supposed to be in a array, fetch the value */
  /* If the field is supposed to be in a array, but the token isn't in a array or vice versa, return 0 */
  if(information->isInArray == true && field->type == JSMN_ARRAY){
    field = getValueFromArray(self, field, information->arrayIndex);
  } else if(information->isInArray ^ (field->type == JSMN_ARRAY)) {
    return 0;
  }

  if(field->type != JSMN_PRIMITIVE){
    return 0;
  }

  char value[20];
  copyString(value, self->storageString + field->start, field->end - field->start);

  return atoi(value);
}

static float getFloatFromCommand(IUnpackerHandle iUnpackHandler, CommandFetchingInformation* information){
  JsonUnpackerHandle self = (JsonUnpackerHandle) iUnpackHandler->implementer;

  jsmntok_t* field = getCommand(self, information->commandName);
  field = getField(self, field, information->fieldName);

  if(field == NULL){
    return 0;
  }

  /* If the field is supposed to be in a array, fetch the value */
  /* If the field is supposed to be in a array, but the token isn't in a array or vice versa, return 0 */
  if(information->isInArray == true && field->type == JSMN_ARRAY){
    field = getValueFromArray(self, field, information->arrayIndex);
  } else if(information->isInArray ^ (field->type == JSMN_ARRAY)) {
    return 0;
  }

  if(field->type != JSMN_PRIMITIVE){
    return 0;
  }

  char value[20];
  copyString(value, self->storageString + field->start, field->end - field->start);

  return atof(value);
}

static bool getBoolFromCommand(IUnpackerHandle iUnpackHandler, CommandFetchingInformation* information){
  JsonUnpackerHandle self = (JsonUnpackerHandle) iUnpackHandler->implementer;

  jsmntok_t* field = getCommand(self, information->commandName);
  field = getField(self, field, information->fieldName);

  if(field == NULL){
    return false;
  }

  /* If the field is supposed to be in a array, fetch the value */
  /* If the field is supposed to be in a array, but the token isn't in a array or vice versa, return 0 */
  if(information->isInArray == true && field->type == JSMN_ARRAY){
    field = getValueFromArray(self, field, information->arrayIndex);
  } else if(information->isInArray ^ (field->type == JSMN_ARRAY)) {
    return false;
  }

  if(field->type != JSMN_STRING){
    return false;
  }

  char value[20];
  copyString(value, self->storageString + field->start, field->end - field->start);

  return strncmp(value, "true", 20) == 0 ? true : false;
}

static void getStringFromCommand(IUnpackerHandle iUnpackHandler, CommandFetchingInformation* information,
                                 char* targetStr,
                                 const int maxLenght){

  JsonUnpackerHandle self = (JsonUnpackerHandle) iUnpackHandler->implementer;

  jsmntok_t* field = getCommand(self, information->commandName);
  field = getField(self, field, information->fieldName);

  if(field == NULL){
    return;
  }

  /* If the field is supposed to be in a array, fetch the value */
  /* If the field is supposed to be in a array, but the token isn't in a array or vice versa, return 0 */
  if(information->isInArray == true && field->type == JSMN_ARRAY){
    field = getValueFromArray(self, field, information->arrayIndex);
  } else if(information->isInArray ^ (field->type == JSMN_ARRAY)) {
    return;
  }

  if(field->type != JSMN_STRING){
    return;
  }

  copyString(targetStr, self->storageString + field->start,(size_t) field->end - field->start);

}

static bool getNameOfField(IUnpackerHandle iUnpackHandler, const char* commandName, char* fieldName,
                           const int maxLenght,
                           const int index){
  JsonUnpackerHandle self = (JsonUnpackerHandle) iUnpackHandler->implementer;
  return false;
}

static size_t getLengthOfCheck(IUnpackerHandle iUnpackHandler){
  JsonUnpackerHandle self = (JsonUnpackerHandle) iUnpackHandler->implementer;

  jsmntok_t* tok = getToken(self->inputString, self->inputTokens, (const char*) "transport", self->msgLength);

  if((tok+1)->type != JSMN_STRING){
    return 0;
  }

  return (size_t)(tok+1)->end - (tok+1)->start;
}

static size_t getLengthOfBytesToCheck(IUnpackerHandle iUnpackHandler){
  JsonUnpackerHandle self = (JsonUnpackerHandle) iUnpackHandler->implementer;

  jsmntok_t* tok = getToken(self->inputString, self->inputTokens, (const char*) "payload", self->msgLength);

  if((tok+1)->type != JSMN_OBJECT){
    return 0;
  }

  return (size_t)(tok+1)->end - (tok+1)->start;
}

static void getBytesToCheck(IUnpackerHandle iUnpackHandler, uint8_t* data){
  JsonUnpackerHandle self = (JsonUnpackerHandle) iUnpackHandler->implementer;
  jsmntok_t* tok = getToken(self->inputString, self->inputTokens, (const char*) "payload", self->msgLength);

  if((tok+1)->type != JSMN_OBJECT){
    return;
  }

  strncpy((char*)data, self->inputString + (tok+1)->start, (size_t)(tok+1)->end - (tok+1)->start);
}

static void getCheck(IUnpackerHandle iUnpackHandler, uint8_t* checkData){
  JsonUnpackerHandle self = (JsonUnpackerHandle) iUnpackHandler->implementer;

  jsmntok_t* tok = getToken(self->inputString, self->inputTokens, (const char*) "transport", self->msgLength);

  if((tok+1)->type != JSMN_STRING){
    return;
  }

  strncpy((char*)checkData, self->inputString + (tok+1)->start, (size_t)(tok+1)->end - (tok+1)->start);
  checkData[(size_t)(tok+1)->end - (tok+1)->start] = '\0';
}

static int getNumberOfFields(IUnpackerHandle iUnpackHandler, const char* commandName){
  JsonUnpackerHandle self = (JsonUnpackerHandle) iUnpackHandler->implementer;
}

/******************************************************************************
 Public functions
******************************************************************************/
JsonUnpackerHandle JsonUnpacker_create(const size_t msgLength){
  
  JsonUnpackerHandle self = (JsonUnpackerHandle) malloc(sizeof(JsonUnpackerPrivateData));

  self->numberOfCommands = 0;
 // self->msgLength = msgLength;
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

  self->msgLength = 100;

  self->inputTokens = (jsmntok_t*) malloc(sizeof(jsmntok_t) * self->msgLength);
  self->storageTokens = (jsmntok_t*) malloc(sizeof(jsmntok_t) * self->msgLength);

  self->inputString = (char*) malloc(sizeof(char) * 500);
  self->storageString = (char*) malloc(sizeof(char) * 500);

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
