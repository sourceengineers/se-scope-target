/*!****************************************************************************************************************************************
 * @file         CommandRunningParser.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Command/CommandRunningParser.h>


typedef struct __CommandRunningParserPrivateData
{
  ICommandHandle iCommand;
  IUnpackerHandle iUnpacker;
  char* commandName;

} CommandRunningParserPrivateData ;


CommandRunningParserHandle CommandRunningParser_create(ICommandHandle iCommand, IUnpackerHandle iUnpacker){
  CommandRunningParserHandle self = malloc(sizeof(CommandRunningParserPrivateData));
  self->iCommand = iCommand;
  self->iUnpacker = iUnpacker;
  self->commandName = (char*) self->iCommand->getCommandName(self->iCommand);
  return self;
}

void CommandRunningParser_configure(CommandRunningParserHandle self){

  size_t numberOfFields = self->iUnpacker->getNumberOfFields(self->iUnpacker, (const char*) self->commandName);

  int channelIds[numberOfFields];
  CHANNEL_STATES newStates[numberOfFields];

  size_t maxSizeOfFieldName;
  char nameOfField[maxSizeOfFieldName];

  for (size_t i = 0; i < numberOfFields; ++i) {

    bool foundField = self->iUnpacker->getNameOfField(self->iUnpacker, self->commandName, nameOfField, maxSizeOfFieldName, i);
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

/* Deconstructor: Deletes the instanze of the channel */
void CommandRunningParser_destroy(CommandRunningParserHandle self){
  free(self);
  self = NULL;
}
