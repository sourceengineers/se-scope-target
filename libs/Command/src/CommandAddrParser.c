/*!****************************************************************************************************************************************
 * @file         CommandAddrParser.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Command/CommandAddrParser.h>
#include <string.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandAddrParserPrivateData
{
  ICommandHandle iCommand;
  IUnpackerHandle iUnpacker;
  char* commandName;

} CommandAddrParserPrivateData ;

/* Takes a String as input and retrieves the matching data type defined in "DataTypes.h" */
static DATA_TYPES parseStringToDataType(const char* dataTypeName, const size_t maxLength);

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
  } else if(strncmp(dataTypeName, FLOAT_NAME, maxLength) == 0){
    return FLOAT;
  }

  return FLOAT;
}

/******************************************************************************
 Public functions
******************************************************************************/
CommandAddrParserHandle CommandAddrParser_create(ICommandHandle iCommand, IUnpackerHandle iUnpacker){
  CommandAddrParserHandle self = malloc(sizeof(CommandAddrParserPrivateData));
  self->iCommand = iCommand;
  self->iUnpacker = iUnpacker;
  self->commandName = (char*) self->iCommand->getCommandName(self->iCommand);
  return self;
}

void CommandAddrParser_configure(CommandAddrParserHandle self){

  if(self->iUnpacker == NULL){
    return;
  }

  ssize_t numberOfFields = self->iUnpacker->getNumberOfFields(self->iUnpacker, (const char*) self->commandName);

  if(numberOfFields == -1){
    return;
  }

  int channelIds[numberOfFields];
  void* newAddresses[numberOfFields];
  DATA_TYPES types[numberOfFields];

  char nameOfField[MAX_FIELD_LENGTH];

  for (size_t i = 0; i < numberOfFields; i++) {

    bool foundField = self->iUnpacker->getNameOfField(self->iUnpacker, self->commandName, nameOfField, MAX_FIELD_LENGTH, i);

    /* Only start parsing if the field was found, and its not a data type field */
    if(foundField == true && strstr(nameOfField, "_type") == NULL){
      channelIds[i] = atoi(nameOfField) - 1;

      newAddresses[i] = (void*) (uint32_t) self->iUnpacker->getIntFromCommand(self->iUnpacker, self->commandName, nameOfField);

      char dataTypeKey[10];
      strncpy(dataTypeKey, nameOfField, 10);
      strncat(dataTypeKey, "_type", 10);
      char requestedDataType[20];
      self->iUnpacker->getStringFromCommand(self->iUnpacker, self->commandName, dataTypeKey, requestedDataType, 20);
      types[i] = parseStringToDataType(requestedDataType, 20);
    }
  }

  CommandAddrConf conf = {.newAddresses = newAddresses, \
                          .changedChannels = channelIds, \
                          .types = types, \
                          .numberOfChangedChannels = numberOfFields / 2};


  self->iCommand->setCommandAttribute(self->iCommand, (void*) &conf);
}

/* Deconstructor: Deletes the instance of the channel */
void CommandAddrParser_destroy(CommandAddrParserHandle self){
  free(self);
  self = NULL;
}
