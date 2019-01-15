/*!****************************************************************************************************************************************
 * @file         CommandAddrParser.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Command/CommandAddrParser.h>
#include <string.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandAddrParserPrivateData
{
  ICommandHandle command;
  IUnpackerHandle unpacker;
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
CommandAddrParserHandle CommandAddrParser_create(ICommandHandle command, IUnpackerHandle unpacker){
  CommandAddrParserHandle self = malloc(sizeof(CommandAddrParserPrivateData));
  self->command = command;
  self->unpacker = unpacker;
  self->commandName = (char*) self->command->getCommandName(self->command);
  return self;
}

void CommandAddrParser_configure(CommandAddrParserHandle self){

  if(self->unpacker == NULL){
    return;
  }

  int numberOfFields = self->unpacker->getNumberOfFields(self->unpacker, (const char*) self->commandName);

  if(numberOfFields == -1){
    return;
  }

  int channelIds[numberOfFields];
  void* newAddresses[numberOfFields];
  DATA_TYPES types[numberOfFields];

  char nameOfField[MAX_FIELD_LENGTH];

  for (size_t i = 0; i < numberOfFields; i++) {

    bool foundField = self->unpacker->getNameOfField(self->unpacker, self->commandName, nameOfField, MAX_FIELD_LENGTH, i);

    /* Only start parsing if the field was found, and its not a data type field */
    if(foundField == true ){
      channelIds[i] = atoi(nameOfField);

      CommandFetchingInformation information = { .commandName = self->commandName, .fieldName = nameOfField,
                                                 .isInArray = true, .arrayIndex = 0 };

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


  self->command->setCommandAttribute(self->command, (void*) &conf);
}

/* Deconstructor: Deletes the instance of the channel */
void CommandAddrParser_destroy(CommandAddrParserHandle self){
  free(self);
  self = NULL;
}
