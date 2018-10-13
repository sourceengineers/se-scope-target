/*!****************************************************************************************************************************************
 * @file         CommandAddrParser.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Command/CommandAddrParser.h>

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

  for (size_t i = 0; i < numberOfFields; ++i) {

    bool foundField = self->iUnpacker->getNameOfField(self->iUnpacker, self->commandName, nameOfField, MAX_FIELD_LENGTH, i);
    if(foundField == true){
      channelIds[i] = atoi(nameOfField) - 1;
      newAddresses[i] = (void*) (uint32_t) self->iUnpacker->getIntFromCommand(self->iUnpacker, self->commandName, nameOfField);
      types[i] = FLOAT;
    }
  }

  CommandAddrConf conf = {.newAddresses = newAddresses, \
                          .changedChannels = channelIds, \
                          .types = types, \
                          .numberOfChangedChannels = numberOfFields};


  self->iCommand->setCommandAttribute(self->iCommand, (void*) &conf);
}

/* Deconstructor: Deletes the instance of the channel */
void CommandAddrParser_destroy(CommandAddrParserHandle self){
  free(self);
  self = NULL;
}