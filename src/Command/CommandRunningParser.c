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
/* Name of the command */
static char* commandName = "cf_running";

/* Class data */
typedef struct __CommandRunningParserPrivateData{
    CommandRunningHandle command;
    IUnpackerHandle unpacker;
    char* commandName;

} CommandRunningParserPrivateData;

/******************************************************************************
 Public functions
******************************************************************************/
CommandRunningParserHandle CommandRunningParser_create(ChannelHandle* channels, size_t amountOfChannels, \
                                                        IUnpackerHandle unpacker){
  CommandRunningParserHandle self = malloc(sizeof(CommandRunningParserPrivateData));
  self->command = CommandRunning_create(channels, amountOfChannels);
  self->unpacker = unpacker;
  return self;
}

ICommandHandle CommandRunningParser_getCommand(CommandRunningParserHandle self){

  if(self->unpacker == NULL){
    return NULL;
  }

  int numberOfFields = self->unpacker->getNumberOfFields(self->unpacker, (const char*) self->commandName);

  if(numberOfFields == -1){
    return NULL;
  }

  int channelIds[numberOfFields];
  CHANNEL_STATES newStates[numberOfFields];

  char nameOfField[MAX_FIELD_LENGTH];

  for(size_t i = 0; i < numberOfFields; ++i){

    bool foundField = self->unpacker->getNameOfField(self->unpacker, self->commandName, nameOfField, MAX_FIELD_LENGTH,
                                                     i);
    if(foundField == true){
      channelIds[i] = atoi(nameOfField);

      CommandFetchingInformation information = {.commandName = self->commandName, .fieldName = nameOfField,
              .isInArray = false, .arrayIndex = 0};

      bool newState = self->unpacker->getBoolFromCommand(self->unpacker, &information);
      if(newState == true){
        newStates[i] = CHANNEL_RUNNING;
      } else{
        newStates[i] = CHANNEL_STOPPED;
      }
    }
  }

  CommandRunningConf conf = {.newStates = newStates, \
                          .changedChannels = channelIds, \
                          .numberOfChangedChannels = numberOfFields};

  CommandRunning_setAttributes(self->command, conf);

  return CommandRunning_getICommand(self->command);
}

char* CommandRunningParser_getName(){
  return commandName;
}

void CommandRunningParser_destroy(CommandRunningParserHandle self){
  CommandRunning_destroy(self->command);

  free(self);
  self = NULL;
}
