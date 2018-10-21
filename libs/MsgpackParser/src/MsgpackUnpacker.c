/*!****************************************************************************************************************************************
 * @file         MsgpackUnpacker.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <MsgpackParser/MsgpackUnpacker.h>
#include <msgpack/object.h>
#include <msgpack.h>
#include <string.h>
#include <GeneralPurpose/Memory.h>
#include <GeneralPurpose/IByteStream.h>

/* Returns a empty msgpack object */
#define returnEmptyObj \
  msgpack_object empty; \
  empty.type = MSGPACK_OBJECT_NIL; \
  return empty;


/* Makro to check if the msgpack_object is a map or not. Returns a empty object if not*/
#define msgpackObjectSanityTest(obj) if(obj.type!=MSGPACK_OBJECT_MAP){returnEmptyObj;}

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __MsgpackUnpackerPrivateData
{

  IUnpacker iUnpacker;

  /* Attributes*/
  msgpack_unpacked und;   /* unpacked data */
  msgpack_unpack_return ret; /* unpacking return data */
  msgpack_object obj;
  msgpack_unpacker unp; /* unpacker */

  msgpack_sbuffer sbuf; /* buffer */
  msgpack_packer pk;    /* packer */

  size_t numberOfCommands;
  size_t msgLength;
} MsgpackUnpackerPrivateData ;

/* Checks if the gives object is a map */
static bool msgpackObjIsMap(msgpack_object obj);

/* Returns a map, broken down to the command map */
static msgpack_object getCommandMap(msgpack_object parentObj);

/* Returns the msgpack_object corresponding to the given key */
static msgpack_object getCmdObj(MsgpackUnpackerHandle self, const char *key);

/* Returns the msgpack_object corresponding to the given key */
static msgpack_object getFieldFromCommand(msgpack_object parentObj, const char *key);

/* Matches the given key to a object on the child level of the gives parent object
    returns a integer indicating the position of the key in the object */
static ssize_t matchKeyToIndex(msgpack_object parentObj, const char *key);

/* Returns msgpack_object matching the the key */
static msgpack_object matchKeyToObj(msgpack_object parentObj, const char *key);

/******************************************************************************
 Private functions
******************************************************************************/
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

static bool unpack(IUnpackerHandle iUnpackHandler, const char* data, const size_t length){

  MsgpackUnpackerHandle self = (MsgpackUnpackerHandle) iUnpackHandler->implementer;

  /* !!!! unpacker has to be reinitialized. It uses malloc for this process which is unfortunate and possibly has to
   * be changed !!! */
  msgpack_unpacker_destroy(&self->unp);
  msgpack_unpacker_init(&self->unp, self->msgLength);
  msgpack_sbuffer_clear(&self->sbuf);

  if (msgpack_unpacker_buffer_capacity(&self->unp) < length) {
    return false;
  }
  copyMemory(msgpack_unpacker_buffer(&self->unp), data, length);
  msgpack_unpacker_buffer_consumed(&self->unp, length);

  self->ret = msgpack_unpacker_next(&self->unp, &self->und);
  switch(self->ret) {
    case MSGPACK_UNPACK_SUCCESS:
    {

      /* Additional test to check if the data was processed correctly */
      if(self->und.data.type == MSGPACK_OBJECT_MAP && getCommandMap(self->und.data).type != MSGPACK_OBJECT_NIL){

        msgpack_object obj = getCommandMap(self->und.data);

        if(obj.type == MSGPACK_OBJECT_NIL) {
          return false;
        }
        if(obj.via.map.size <= 0){
          return false;
        }

        msgpack_pack_object(&self->pk, obj);

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

static void activateNewMessage(IUnpackerHandle iUnpackHandler){
  MsgpackUnpackerHandle self = (MsgpackUnpackerHandle) iUnpackHandler->implementer;

  self->obj = self->und.data;
  self->numberOfCommands = getCommandMap(self->und.data).via.map.size;
}

static msgpack_object getCmdObj(MsgpackUnpackerHandle self, const char *key){

  msgpack_object obj = getCommandMap(self->obj);
  msgpackObjectSanityTest(obj);
  obj = matchKeyToObj(obj, key);

  return obj;
}

static msgpack_object getFieldFromCommand(msgpack_object parentObj, const char *key){

  msgpackObjectSanityTest(parentObj);

  /* Returns the parent object if no key is given */
  if(strcmp(key,"") == 0){
    return parentObj;
  }

  const ssize_t offset = matchKeyToIndex(parentObj, key);

  /* Returns a empty object if no key could be matched */
  if(offset == -1){
    msgpack_object empty;
    empty.type = MSGPACK_OBJECT_NIL;
    return empty;
  }

  return (parentObj.via.map.ptr+offset)->val;
}

static const size_t getNumberOfCommands(IUnpackerHandle iUnpackHandler){
  MsgpackUnpackerHandle self = (MsgpackUnpackerHandle) iUnpackHandler->implementer;

  return self->numberOfCommands;
}

static ssize_t matchKeyToIndex(msgpack_object parentObj, const char *key){

  /* Returns an error the object is not a map */
  if(parentObj.type != MSGPACK_OBJECT_MAP){
    return -1;
  }

  /* Check all objects for the given key */
  for(int i = 0; i<parentObj.via.map.size; i++){
    msgpack_object objKey = (parentObj.via.map.ptr+i)->key;

    if(objKey.type != MSGPACK_OBJECT_STR){
      return -1;
    }

    char tmpKey[objKey.via.str.size + 1];
    copyString(tmpKey, (char*) objKey.via.str.ptr, objKey.via.str.size);

    if((strncmp(tmpKey, key, objKey.via.str.size) == 0) && (strlen(key) == objKey.via.str.size)){
      return i;
    }
  }
  return -1;
}

static bool getNameOfCommand(IUnpackerHandle iUnpackHandler, char* name, const int maxLenght, const int index){
  MsgpackUnpackerHandle self = (MsgpackUnpackerHandle) iUnpackHandler->implementer;

  /* Yeilds an eror if the index is negative */
  if(index < 0){
    return false;
  }

  msgpack_object obj = getCommandMap(self->obj);

  /* Make sure the object is a map */
  if(obj.type != MSGPACK_OBJECT_MAP){
    return false;
  }

  /* Get the key if name of the command */
  obj = (obj.via.map.ptr+index)->key;

  /* Yield error if the command name is too big */
  if(obj.via.str.size > maxLenght){
    return false;
  }

  copyString(name, (char *)obj.via.str.ptr, obj.via.str.size);

  return true;
}

static msgpack_object matchKeyToObj(msgpack_object parentObj, const char *key){

  /* If there is no field argument given, it is assumed, that the command has no fields and is the field itself */
  if(strcmp(key, "") == 0){
    return parentObj;
  }

  const ssize_t offset = matchKeyToIndex(parentObj, key);

  /* Return empty object if offset is negative */
  if(offset < 0){
    returnEmptyObj;
  }

  return (parentObj.via.map.ptr+offset)->val;
}

static uint32_t getIntFromCommand(IUnpackerHandle iUnpackHandler, const char* commandName, const char* fieldName){
  MsgpackUnpackerHandle self = (MsgpackUnpackerHandle) iUnpackHandler->implementer;

  msgpack_object obj = getCmdObj(self, commandName);

  if((msgpackObjIsMap(obj) == true)){
      obj = getFieldFromCommand(obj, fieldName);
  }

  /* Return 0 if the object is not integer */
  if ((obj.type != MSGPACK_OBJECT_NEGATIVE_INTEGER) && (obj.type != MSGPACK_OBJECT_POSITIVE_INTEGER)){
    return 0;
  }

  return (uint32_t) obj.via.i64;
}

static float getFloatFromCommand(IUnpackerHandle iUnpackHandler,const char* commandName, const char* fieldName){
  MsgpackUnpackerHandle self = (MsgpackUnpackerHandle) iUnpackHandler->implementer;

  msgpack_object obj = getCmdObj(self, commandName);

  /* Return zero if the object isn't the right type */
  if(msgpackObjIsMap(obj) == false){
    return 0.0f;
  }

  obj = getFieldFromCommand(obj, fieldName);

  /* Return zero if the object isn't the right type */
  if((obj.type != MSGPACK_OBJECT_FLOAT) && (obj.type != MSGPACK_OBJECT_FLOAT32) && (obj.type != MSGPACK_OBJECT_FLOAT64)){
    return 0.0f;
  }

  return (float) obj.via.f64;
}

static bool getBoolFromCommand(IUnpackerHandle iUnpackHandler,const char* commandName, const char* fieldName){
  MsgpackUnpackerHandle self = (MsgpackUnpackerHandle) iUnpackHandler->implementer;

  msgpack_object obj = getCmdObj(self, commandName);

  /* Return false if the object isn't the right type */
  if(msgpackObjIsMap(obj) == false){
    return false;
  }

  obj = getFieldFromCommand(obj, fieldName);

  /* Return false if the object isn't the right type */
  if(obj.type != MSGPACK_OBJECT_BOOLEAN){
    return false;
  }

  return obj.via.boolean;
}

static void getStringFromCommand(IUnpackerHandle iUnpackHandler,const char* commandName, const char* fieldName,
                                 char* targetStr,
                                 const int maxLenght){

  MsgpackUnpackerHandle self = (MsgpackUnpackerHandle) iUnpackHandler->implementer;

  msgpack_object obj = getCmdObj(self, commandName);

  /* Return false if the object isn't the right type */
  if(msgpackObjIsMap(obj) == false) {
    return;
  }

  obj = getFieldFromCommand(obj, fieldName);

  /* Return false if the object isn't the right type */
  if(obj.type != MSGPACK_OBJECT_STR) {
    return;
  }

  int strLength = obj.via.str.size;

  /* Copy string */
  if(maxLenght >= strLength){
    strncpy(targetStr, obj.via.str.ptr, strLength);
    targetStr[strLength] = '\0';
  } else {
    targetStr[0] = '\0';
  }
}

static bool getNameOfField(IUnpackerHandle iUnpackHandler, const char* commandName, char* fieldName,
                           const int maxLenght,
                           const int index){
  MsgpackUnpackerHandle self = (MsgpackUnpackerHandle) iUnpackHandler->implementer;

  /* Return error if index is negative */
  if(index < 0){
    return false;
  }

  msgpack_object obj = getCmdObj(self, commandName);

  /* Return error if object isn't the right type */
  if(msgpackObjIsMap(obj) == false) {
    return false;
  }

  obj = (obj.via.map.ptr+index)->key;

  /* Return error if the buffer isn't big enough */
  if(obj.via.str.size > maxLenght){
    return false;
  }

  copyString(fieldName, (char *)obj.via.str.ptr, obj.via.str.size);

  return true;
}

static size_t getLengthOfCheck(IUnpackerHandle iUnpackHandler){
  MsgpackUnpackerHandle self = (MsgpackUnpackerHandle) iUnpackHandler->implementer;

  msgpack_object transportObject = matchKeyToObj(self->obj, (const char*) "transport");

  /* Return false if the object isn't the right type */
  if(transportObject.type != MSGPACK_OBJECT_BIN) {
    return 0;
  }
  return transportObject.via.bin.size;
}

static size_t getLengthOfBytesToCheck(IUnpackerHandle iUnpackHandler){
  MsgpackUnpackerHandle self = (MsgpackUnpackerHandle) iUnpackHandler->implementer;

  return self->sbuf.size;
}

static void getBytesToCheck(IUnpackerHandle iUnpackHandler, uint8_t* data){
  MsgpackUnpackerHandle self = (MsgpackUnpackerHandle) iUnpackHandler->implementer;

  copyMemory((char*) data, self->sbuf.data, self->sbuf.size);
}

static void getCheck(IUnpackerHandle iUnpackHandler, uint8_t* checkData){
  MsgpackUnpackerHandle self = (MsgpackUnpackerHandle) iUnpackHandler->implementer;

  msgpack_object transportObject = matchKeyToObj(self->obj, (const char*) "transport");

  /* Return false if the object isn't the right type */
  if(transportObject.type != MSGPACK_OBJECT_BIN) {
    return;
  }

  copyMemory((char*) checkData, transportObject.via.bin.ptr, transportObject.via.bin.size);
}

static ssize_t getNumberOfFields(IUnpackerHandle iUnpackHandler, const char* commandName){
  MsgpackUnpackerHandle self = (MsgpackUnpackerHandle) iUnpackHandler->implementer;

  msgpack_object obj = getCmdObj(self, commandName);

  if(msgpackObjIsMap(obj) == false) {
    return -1;
  }

  return (obj.via.map.size);
}

/******************************************************************************
 Public functions
******************************************************************************/
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
  self->iUnpacker.activateNewMessage = &activateNewMessage;
  self->iUnpacker.getLengthOfBytesToCheck = &getLengthOfBytesToCheck;
  self->iUnpacker.getLengthOfCheck = &getLengthOfCheck;
  self->iUnpacker.getCheck = &getCheck;
  self->iUnpacker.getBytesToCheck = &getBytesToCheck;

  bool result = msgpack_unpacker_init(&self->unp, self->msgLength);

  if(result == false){
    return NULL;
  }

  msgpack_unpacked_init(&self->und);

  /* The packer functions are used to provide data to the IValidator interface */
  msgpack_sbuffer_init(&self->sbuf);
  msgpack_packer_init(&self->pk, &self->sbuf, msgpack_sbuffer_write);
  return self;
}

void MsgpackUnpacker_destroy(MsgpackUnpackerHandle self){
  msgpack_unpacked_destroy(&self->und);
  msgpack_unpacker_destroy(&self->unp);
  msgpack_sbuffer_destroy(&self->sbuf);
  free(self);
  self = NULL;
}

IUnpackerHandle MsgpackUnpacker_getIUnpacker(MsgpackUnpackerHandle self){
  return &self->iUnpacker;
}