/*!****************************************************************************************************************************************
 * @file         msgpack_unpacker.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Author <Email>
 *
 * @brief        TODO
 *
 *****************************************************************************************************************************************/

#include <MsgpackParser/MsgpackUnpacker.h>
#include <msgpack/object.h>



static msgpack_object getCommandMap(MsgpackUnpackerHandle self);
static bool unpack(IParserHandle iParserHandle, const char* data, const int length);
static msgpack_object getCmdObj(MsgpackUnpackerHandle self, const char *key);
static const size_t getNumberOfCommands(IParserHandle iParserHandle);
static void copyString(char *str, char *data, int size);
static size_t matchKeyToIndex(msgpack_object parentObj, const char *key);
static bool getNameOfCommand(IParserHandle iParserHandle, char* name, const int maxLenght, const int index);
static msgpack_object matchKeyToObj(msgpack_object parentObj, const char *key);
static int getIntFromCommand(IParserHandle iParserHandle, const char* commandName, const int offset);
static float getFloatFromCommand(IParserHandle iParserHandle,const char* commandName, const int offset);
static bool getBoolFromCommand(IParserHandle iParserHandle,const char* commandName, const int offset);
static void getStringFromCommand(IParserHandle iParserHandle,const char* commandName, const int offset, char* targetStr,
                                 const int maxLenght);


typedef struct __MsgpackUnpackerPrivateData
{

  IParser iParser;

  /* Attributes*/
  msgpack_unpacked und;   /* unpacked data */
  msgpack_unpack_return ret; /* unpacking return data */
  msgpack_object obj;
  msgpack_unpacker unp;

  size_t numberOfCommands;
  size_t msgLength;
} MsgpackUnpackerPrivateData ;


static msgpack_object getCommandMap(MsgpackUnpackerHandle self){

  msgpack_object obj;
  obj = matchKeyToObj(self->obj, "payload");
  obj = matchKeyToObj(obj, "sc_cmd");
  return obj;
}

/* Unpacks the data and safes it */
static bool unpack(IParserHandle iParserHandle, const char* data, const int length){

  MsgpackUnpackerHandle self = (MsgpackUnpackerHandle) iParserHandle->implementer;


  if (msgpack_unpacker_buffer_capacity(&self->unp) < length) {
    return false;
  }
  memcpy(msgpack_unpacker_buffer(&self->unp), data, length);
  msgpack_unpacker_buffer_consumed(&self->unp, length);

  self->ret = msgpack_unpacker_next(&self->unp, &self->und);
  switch(self->ret) {
    case MSGPACK_UNPACK_SUCCESS:
    {
      self->obj = self->und.data;

      msgpack_object obj = getCommandMap(self);
      self->numberOfCommands = obj.via.map.size;

      return true;
      break;
    }
    case MSGPACK_UNPACK_CONTINUE:
      return false;
      break;
    case MSGPACK_UNPACK_PARSE_ERROR:
      return false;
      break;
  }
  return false;
}


/* Returns the msgpack_object corresponding to the given key */
static msgpack_object getCmdObj(MsgpackUnpackerHandle self, const char *key){
  msgpack_object obj = getCommandMap(self);
  return matchKeyToObj(obj, key);
}

static const size_t getNumberOfCommands(IParserHandle iParserHandle){
  MsgpackUnpackerHandle self = (MsgpackUnpackerHandle) iParserHandle->implementer;

  return self->numberOfCommands;
}


/* Copies the msgpack_object key to key */
 static void copyString(char *str, char *data, int size){
  strncpy(str, data, size);
  str[size] = '\0';
}

/* Matches the given key to a object on the child level of the gives parent object
    returns a integer indicating the position of the key in the object */
static size_t matchKeyToIndex(msgpack_object parentObj, const char *key){
  for(int i = 0; i<parentObj.via.map.size; i++){
    msgpack_object objKey = (parentObj.via.map.ptr+i)->key;

    char tmpKey[objKey.via.str.size + 1];
    copyString(tmpKey, (char *)objKey.via.str.ptr, objKey.via.str.size);

    if(strncmp(tmpKey, key, objKey.via.str.size) == 0){
      return i;
    }
  }
  return -1;
}

/* Returns the name of a command at a given index */
static bool getNameOfCommand(IParserHandle iParserHandle, char* name, const int maxLenght, const int index){
  MsgpackUnpackerHandle self = (MsgpackUnpackerHandle) iParserHandle->implementer;

  msgpack_object obj = getCommandMap(self);
  obj = (obj.via.map.ptr+index)->key;


  if(obj.via.str.size > maxLenght){
    return false;
  }

  copyString(name, (char *)obj.via.str.ptr, obj.via.str.size);

  return true;
}

/* Returns msgpack_object matching the the key */
static msgpack_object matchKeyToObj(msgpack_object parentObj, const char *key){

  int offset = matchKeyToIndex(parentObj, key);
  msgpack_object obj;

  if(offset >= 0 ){
    return (parentObj.via.map.ptr+offset)->val;
  }
  return obj;
}

static int getIntFromCommand(IParserHandle iParserHandle, const char* commandName, const int offset){
  MsgpackUnpackerHandle self = (MsgpackUnpackerHandle) iParserHandle->implementer;

  msgpack_object obj = getCmdObj(self, commandName);

  return (obj.via.map.ptr+offset)->val.via.i64;
}


static float getFloatFromCommand(IParserHandle iParserHandle,const char* commandName, const int offset){
  MsgpackUnpackerHandle self = (MsgpackUnpackerHandle) iParserHandle->implementer;

  msgpack_object obj = getCmdObj(self, commandName);

  return (obj.via.map.ptr+offset)->val.via.f64;
}

static bool getBoolFromCommand(IParserHandle iParserHandle,const char* commandName, const int offset){
  MsgpackUnpackerHandle self = (MsgpackUnpackerHandle) iParserHandle->implementer;

  msgpack_object obj = getCmdObj(self, commandName);

  return (obj.via.map.ptr+offset)->val.via.boolean;
}

static void getStringFromCommand(IParserHandle iParserHandle,const char* commandName, const int offset, char* targetStr,
                                 const int maxLenght){

  MsgpackUnpackerHandle self = (MsgpackUnpackerHandle) iParserHandle->implementer;

  msgpack_object obj = getCmdObj(self, commandName);

  int strLength = (obj.via.map.ptr+offset)->val.via.str.size;

  if(maxLenght >= strLength){
    strncpy(targetStr, (obj.via.map.ptr+offset)->val.via.str.ptr, strLength);
    targetStr[strLength] = '\0';
  } else {
    targetStr[0] = '\0';
  }
}


MsgpackUnpackerHandle MsgpackUnpacker_create(const size_t msgLength){
  
  MsgpackUnpackerHandle self = (MsgpackUnpackerHandle) malloc(sizeof(MsgpackUnpackerPrivateData));

  self->numberOfCommands = 0;
  self->msgLength = msgLength;
  self->iParser.implementer = self;
  self->iParser.unpack = &unpack;
  self->iParser.getBoolFromCommand = &getBoolFromCommand;
  self->iParser.getFloatFromCommand = &getFloatFromCommand;
  self->iParser.getIntFromCommand = &getIntFromCommand;
  self->iParser.getStringFromCommand = &getStringFromCommand;
  self->iParser.getNameOfCommand = &getNameOfCommand;
  self->iParser.getNumberOfCommands = &getNumberOfCommands;


  bool result = msgpack_unpacker_init(&self->unp, self->msgLength);

  if(result == false){
    return NULL;
  }

  msgpack_unpacked_init(&self->und);

  return self;
}

void MsgpackUnpacker_destroy(MsgpackUnpackerHandle self){
  free(self);
  self = NULL;
}

IParserHandle MsgpackUnpacker_getIParser(MsgpackUnpackerHandle self){
  return &self->iParser;
}