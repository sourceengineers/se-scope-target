/*!****************************************************************************************************************************************
 * @file         CommandAddrParser.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Control/CommandParser/CommandAddrParser.h>
#include <string.h>

/******************************************************************************
 Define private data
******************************************************************************/
//* Name of the command */
static char* commandName = "cf_addr";

/* Class data */
typedef struct __CommandAddrParserPrivateData{
    CommandAddrHandle command;
    IUnpackerHandle unpacker;

} CommandAddrParserPrivateData;

/* Takes a String as input and retrieves the matching data type defined in "DataTypes.h" */
static DATA_TYPES parseStringToDataType(const char* dataTypeName, size_t maxLength);

/******************************************************************************
 Private functions
******************************************************************************/
static DATA_TYPES parseStringToDataType(const char* dataTypeName, const size_t maxLength){

  if(strncmp(dataTypeName, UINT8_NAME, maxLength) == 0){
    return UINT8;
  } else if(strncmp(dataTypeName, UINT16_NAME, maxLength) == 0){
    return UINT16;
  } else if(strncmp(dataTypeName, UINT32_NAME, maxLength) == 0){
    return UINT32;
  } else if(strncmp(dataTypeName, UINT64_NAME, maxLength) == 0){
    return UINT64;
  } else if(strncmp(dataTypeName, FLOAT_NAME, maxLength) == 0){
    return FLOAT;
  }

  return FLOAT;
}

/******************************************************************************
 Public functions
******************************************************************************/
CommandAddrParserHandle CommandAddrParser_create(IScopeHandle scope, IUnpackerHandle unpacker){
  CommandAddrParserHandle self = malloc(sizeof(CommandAddrParserPrivateData));
  self->command = CommandAddr_create(scope);
  self->unpacker = unpacker;
  return self;
}

ICommandHandle CommandAddrParser_getCommand(CommandAddrParserHandle self){

  if(self->unpacker == NULL){
    return NULL;
  }

  const uint32_t numberOfFields = self->unpacker->getNumberOfFields(self->unpacker, commandName);

  if(numberOfFields == -1){
    return NULL;
  }

  uint32_t channelIds[numberOfFields];
  void* newAddresses[numberOfFields];
  DATA_TYPES types[numberOfFields];

  char nameOfField[MAX_FIELD_LENGTH];

  for(size_t i = 0; i < numberOfFields; i++){

    const bool foundField = self->unpacker->getNameOfField(self->unpacker, commandName, nameOfField, MAX_FIELD_LENGTH, i);

    /* Only start parsing if the field was found, and its not a data type field */
    if(foundField == true){
      channelIds[i] = (uint32_t) atoi(nameOfField);

      CommandFetchingInformation information = {.commandName = commandName, .fieldName = nameOfField,
              .isInArray = true, .arrayIndex = 0};

      newAddresses[i] = (void*) (ADDRESS_DATA_TYPE) self->unpacker->getIntFromCommand(self->unpacker, &information);


      char requestedDataType[20];
      information.arrayIndex = 1;
      self->unpacker->getStringFromCommand(self->unpacker, &information, requestedDataType, 20);
      types[i] = parseStringToDataType(requestedDataType, 20);
    }
  }

  CommandAddrConf conf = {.newAddresses = newAddresses, \
                          .changedChannels = channelIds, \
                          .types = types, \
                          .numberOfChangedChannels = numberOfFields};

  CommandAddr_setAttributes(self->command, conf);

  return CommandAddr_getICommand(self->command);
}

/* Deconstructor: Deletes the instance of the channel */
void CommandAddrParser_destroy(CommandAddrParserHandle self){
  CommandAddr_destroy(self->command);

  free(self);
  self = NULL;
}

char* CommandAddrParser_getName(){
  return commandName;
}