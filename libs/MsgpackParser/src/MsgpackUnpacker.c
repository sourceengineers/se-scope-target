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
#include <msgpack.h>

/* Returns a empty msgpack object */
#define returnEmptyObj \
  msgpack_object empty; \
  empty.type = MSGPACK_OBJECT_NIL; \
  return empty;


/* Makro to check if the msgpack_object is a map or not. Returns a empty object if not*/
#define msgpackObjectSanityTest(obj) if(obj.type!=MSGPACK_OBJECT_MAP){returnEmptyObj;}


static msgpack_object getCommandMap(msgpack_object parentObj);
static bool unpack(IUnpackerHandle iUnpackHandler, const char* data, const int length);
static msgpack_object getCmdObj(MsgpackUnpackerHandle self, const char *key);
static const size_t getNumberOfCommands(IUnpackerHandle iUnpackHandler);
static void copyString(char *str, char *data, int size);
static size_t matchKeyToIndex(msgpack_object parentObj, const char *key);
static bool getNameOfCommand(IUnpackerHandle iUnpackHandler, char* name, const int maxLenght, const int index);
static msgpack_object matchKeyToObj(msgpack_object parentObj, const char *key);
static int getIntFromCommand(IUnpackerHandle iUnpackHandler, const char* commandName, const char* fieldName);
static float getFloatFromCommand(IUnpackerHandle iUnpackHandler,const char* commandName, const char* fieldName);
static bool getBoolFromCommand(IUnpackerHandle iUnpackHandler,const char* commandName, const char* fieldName);
static void getStringFromCommand(IUnpackerHandle iUnpackHandler,const char* commandName, const char* fieldName,
                                 char* targetStr,
                                 const int maxLenght);


typedef struct __MsgpackUnpackerPrivateData
{

  IUnpacker iUnpacker;

  /* Attributes*/
  msgpack_unpacked und;   /* unpacked data */
  msgpack_unpack_return ret; /* unpacking return data */
  msgpack_object obj;
  msgpack_unpacker unp;

  size_t numberOfCommands;
  size_t msgLength;
} MsgpackUnpackerPrivateData ;

static bool msgpackObjIsMap(msgpack_object obj){
  return obj.type != MSGPACK_OBJECT_MAP ? false : true;
}

static msgpack_object getCommandMap(msgpack_object parentObj){

  msgpack_object obj;

  obj = matchKeyToObj(parentObj, "payload");
  msgpackObjectSanityTest(obj);
  obj = matchKeyToObj(obj, "sc_cmd");
  msgpackObjectSanityTest(obj);

  return obj;
}

/* Unpacks the data and safes it */
static bool unpack(IUnpackerHandle iUnpackHandler, const char* data, const int length){

  MsgpackUnpackerHandle self = (MsgpackUnpackerHandle) iUnpackHandler->implementer;

  /* !!!! unpacker has to be reinitialized. It uses malloc for this process which is unfortunate and possibly has to
   * be changed !!! */
  msgpack_unpacker_destroy(&self->unp);
  msgpack_unpacker_init(&self->unp, self->msgLength);

  if (msgpack_unpacker_buffer_capacity(&self->unp) < length) {
    return false;
  }
  memcpy(msgpack_unpacker_buffer(&self->unp), data, length);
  msgpack_unpacker_buffer_consumed(&self->unp, length);

  self->ret = msgpack_unpacker_next(&self->unp, &self->und);
  switch(self->ret) {
    case MSGPACK_UNPACK_SUCCESS:
    {

      /* Additional test to check if the data was processed correctly */
      if(self->und.data.type == MSGPACK_OBJECT_MAP && getCommandMap(self->und.data).type != MSGPACK_OBJECT_NIL){

        self->obj = self->und.data;

        msgpack_object obj = getCommandMap(self->obj);

        if(obj.type == MSGPACK_OBJECT_NIL) {
          return false;
        }
        if(obj.via.map.size <= 0){
          return false;
        }

        self->numberOfCommands = obj.via.map.size;

        return true;
      }

      return false;
      break;
    }
    case MSGPACK_UNPACK_PARSE_ERROR:
      return false;
      break;
  }
  return false;
}

/* Returns the msgpack_object corresponding to the given key */
static msgpack_object getCmdObj(MsgpackUnpackerHandle self, const char *key){

  msgpack_object obj = getCommandMap(self->obj);
  msgpackObjectSanityTest(obj);
  obj = matchKeyToObj(obj, key);

  return obj;
}

/* Returns the msgpack_object corresponding to the given key */
static msgpack_object getFieldFromCommand(msgpack_object parentObj, const char *key){

  msgpackObjectSanityTest(parentObj);

  if(strcmp(key,"") == 0){
    return parentObj;
  }

  size_t offset = matchKeyToIndex(parentObj, key);

  if(offset == -1){
    msgpack_object empty;
    empty.type = MSGPACK_OBJECT_NIL;
    return empty;
  }

  return (parentObj.via.map.ptr+offset)->val;
}


/* Returns the ammount of numbers parsed by the unpacker */
static const size_t getNumberOfCommands(IUnpackerHandle iUnpackHandler){
  MsgpackUnpackerHandle self = (MsgpackUnpackerHandle) iUnpackHandler->implementer;

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

  if(parentObj.type != MSGPACK_OBJECT_MAP){
    return -1;
  }

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
static bool getNameOfCommand(IUnpackerHandle iUnpackHandler, char* name, const int maxLenght, const int index){
  MsgpackUnpackerHandle self = (MsgpackUnpackerHandle) iUnpackHandler->implementer;

  if(index < 0){
    return false;
  }

  msgpack_object obj = getCommandMap(self->obj);

  if(obj.type != MSGPACK_OBJECT_MAP){
    return false;
  }

  obj = (obj.via.map.ptr+index)->key;

  if(obj.via.str.size > maxLenght){
    return false;
  }

  copyString(name, (char *)obj.via.str.ptr, obj.via.str.size);

  return true;
}

/* Returns msgpack_object matching the the key */
static msgpack_object matchKeyToObj(msgpack_object parentObj, const char *key){

  /* If there is no field argument given, it is assumed, that the command has no fields and is the field itself */
  if(strcmp(key, "") == 0){
    return parentObj;
  }

  int offset = matchKeyToIndex(parentObj, key);

  if(offset < 0){
    returnEmptyObj;
  }

  return (parentObj.via.map.ptr+offset)->val;

}

/* Returns the value located at the Command field */
static int getIntFromCommand(IUnpackerHandle iUnpackHandler, const char* commandName, const char* fieldName){
  MsgpackUnpackerHandle self = (MsgpackUnpackerHandle) iUnpackHandler->implementer;

  msgpack_object obj = getCmdObj(self, commandName);

  if((msgpackObjIsMap(obj) == true)){
      obj = getFieldFromCommand(obj, fieldName);
  }

  if ((obj.type != MSGPACK_OBJECT_NEGATIVE_INTEGER) && (obj.type != MSGPACK_OBJECT_POSITIVE_INTEGER)){
    return 0;
  }

  return obj.via.i64;
}


/* Returns the value located at the Command field */
static float getFloatFromCommand(IUnpackerHandle iUnpackHandler,const char* commandName, const char* fieldName){
  MsgpackUnpackerHandle self = (MsgpackUnpackerHandle) iUnpackHandler->implementer;

  msgpack_object obj = getCmdObj(self, commandName);

  if(msgpackObjIsMap(obj) == false){
    return 0.0f;
  }

  obj = getFieldFromCommand(obj, fieldName);


  if((obj.type != MSGPACK_OBJECT_FLOAT) && (obj.type != MSGPACK_OBJECT_FLOAT32) && (obj.type != MSGPACK_OBJECT_FLOAT64)){
    return 0.0f;
  }

  return obj.via.f64;
}

/* Returns the value located at the Command field */
static bool getBoolFromCommand(IUnpackerHandle iUnpackHandler,const char* commandName, const char* fieldName){
  MsgpackUnpackerHandle self = (MsgpackUnpackerHandle) iUnpackHandler->implementer;

  msgpack_object obj = getCmdObj(self, commandName);

  if(msgpackObjIsMap(obj) == false){
    return false;
  }

  obj = getFieldFromCommand(obj, fieldName);


  if(obj.type != MSGPACK_OBJECT_BOOLEAN){
    return false;
  }

  return obj.via.boolean;
}

/* Returns the name of a field of a command at a specific location */
static void getStringFromCommand(IUnpackerHandle iUnpackHandler,const char* commandName, const char* fieldName,
                                 char* targetStr,
                                 const int maxLenght){

  MsgpackUnpackerHandle self = (MsgpackUnpackerHandle) iUnpackHandler->implementer;

  msgpack_object obj = getCmdObj(self, commandName);

  if(msgpackObjIsMap(obj) == false) {
    return;
  }

  obj = getFieldFromCommand(obj, fieldName);

  if(obj.type != MSGPACK_OBJECT_STR) {
    return;
  }

  int strLength = obj.via.str.size;

  if(maxLenght >= strLength){
    strncpy(targetStr, obj.via.str.ptr, strLength);
    targetStr[strLength] = '\0';
  } else {
    targetStr[0] = '\0';
  }
}

/* Returns the name of a field of a command at a specific location */
static bool getNameOfField(IUnpackerHandle iUnpackHandler, const char* commandName, char* fieldName,
                           const int maxLenght,
                           const int index){
  MsgpackUnpackerHandle self = (MsgpackUnpackerHandle) iUnpackHandler->implementer;

  if(index < 0){
    return false;
  }

  msgpack_object obj = getCmdObj(self, commandName);

  if(msgpackObjIsMap(obj) == false) {
    return false;
  }

  obj = (obj.via.map.ptr+index)->key;

  if(obj.via.str.size > maxLenght){
    return false;
  }

  copyString(fieldName, (char *)obj.via.str.ptr, obj.via.str.size);

  return true;
}

/* Returns the number of fields a command has */
static size_t getNumberOfFields(IUnpackerHandle iUnpackHandler, const char* commandName){
  MsgpackUnpackerHandle self = (MsgpackUnpackerHandle) iUnpackHandler->implementer;

  msgpack_object obj = getCmdObj(self, commandName);

  if(msgpackObjIsMap(obj) == false) {
    return -1;
  }

  return (obj.via.map.size);
}


MsgpackUnpackerHandle MsgpackUnpacker_create(const size_t msgLength){
  
  MsgpackUnpackerHandle self = (MsgpackUnpackerHandle) malloc(sizeof(MsgpackUnpackerPrivateData));

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


  bool result = msgpack_unpacker_init(&self->unp, self->msgLength);

  if(result == false){
    return NULL;
  }

  msgpack_unpacked_init(&self->und);

  return self;
}

void MsgpackUnpacker_destroy(MsgpackUnpackerHandle self){
  msgpack_unpacked_destroy(&self->und);
  msgpack_unpacker_destroy(&self->unp);
  free(self);
  self = NULL;
}

IUnpackerHandle MsgpackUnpacker_getIUnpacker(MsgpackUnpackerHandle self){
  return &self->iUnpacker;
}