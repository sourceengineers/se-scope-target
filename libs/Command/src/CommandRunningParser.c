/*!****************************************************************************************************************************************
 * @file         CommandRunningParser.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Command/CommandRunningParser.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandRunningParserPrivateData
{
  ICommandHandle iCommand;
  IUnpackerHandle iUnpacker;
  char* commandName;

} CommandRunningParserPrivateData ;

/******************************************************************************
 Public functions
******************************************************************************/
CommandRunningParserHandle CommandRunningParser_create(ICommandHandle iCommand, IUnpackerHandle iUnpacker){
  CommandRunningParserHandle self = malloc(sizeof(CommandRunningParserPrivateData));
  self->iCommand = iCommand;
  self->iUnpacker = iUnpacker;
  self->commandName = (char*) self->iCommand->getCommandName(self->iCommand);
  return self;
}

void CommandRunningParser_configure(CommandRunningParserHandle self){

  if(self->iUnpacker == NULL){
    return;
  }

  ssize_t numberOfFields = self->iUnpacker->getNumberOfFields(self->iUnpacker, (const char*) self->commandName);

  if(numberOfFields == -1){
    return;
  }

  int channelIds[numberOfFields];
  CHANNEL_STATES newStates[numberOfFields];

  char nameOfField[MAX_FIELD_LENGTH];

  for (size_t i = 0; i < numberOfFields; ++i) {

    bool foundField = self->iUnpacker->getNameOfField(self->iUnpacker, self->commandName, nameOfField, MAX_FIELD_LENGTH, i);
    if(foundField == true){
      channelIds[i] = atoi(nameOfField);
      bool newState = self->iUnpacker->getBoolFromCommand(self->iUnpacker, self->commandName, nameOfField);
      if(newState == true){
        newStates[i] = CHANNEL_RUNNING;
      } else {
        newStates[i] = CHANNEL_STOPPED;
      }
    }
  }

  CommandRunningConf conf = {.newStates = newStates, \
                          .changedChannels = channelIds, \
                          .numberOfChangedChannels = numberOfFields};


  self->iCommand->setCommandAttribute(self->iCommand, (void*) &conf);
}

void CommandRunningParser_destroy(CommandRunningParserHandle self){
  free(self);
  self = NULL;
}
