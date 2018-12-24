/*!****************************************************************************************************************************************
 * @file         CommandRunningParser.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Command/CommandRunningParser.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandRunningParserPrivateData
{
  ICommandHandle command;
  IUnpackerHandle unpacker;
  char* commandName;

} CommandRunningParserPrivateData ;

/******************************************************************************
 Public functions
******************************************************************************/
CommandRunningParserHandle CommandRunningParser_create(ICommandHandle command, IUnpackerHandle unpacker){
  CommandRunningParserHandle self = malloc(sizeof(CommandRunningParserPrivateData));
  self->command = command;
  self->unpacker = unpacker;
  self->commandName = (char*) self->command->getCommandName(self->command);
  return self;
}

void CommandRunningParser_configure(CommandRunningParserHandle self){

  if(self->unpacker == NULL){
    return;
  }

  int numberOfFields = self->unpacker->getNumberOfFields(self->unpacker, (const char*) self->commandName);

  if(numberOfFields == -1){
    return;
  }

  int channelIds[numberOfFields];
  CHANNEL_STATES newStates[numberOfFields];

  char nameOfField[MAX_FIELD_LENGTH];

  for (size_t i = 0; i < numberOfFields; ++i) {

    bool foundField = self->unpacker->getNameOfField(self->unpacker, self->commandName, nameOfField, MAX_FIELD_LENGTH, i);
    if(foundField == true){
      channelIds[i] = atoi(nameOfField);

      CommandFetchingInformation information = { .commandName = self->commandName, .fieldName = nameOfField,
                                                 .isInArray = false, .arrayIndex = 0 };

      bool newState = self->unpacker->getBoolFromCommand(self->unpacker, &information);
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


  self->command->setCommandAttribute(self->command, (void*) &conf);
}

void CommandRunningParser_destroy(CommandRunningParserHandle self){
  free(self);
  self = NULL;
}
