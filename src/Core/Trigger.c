/*!*****************************************************************************
 * @file         Trigger.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 * 
 ******************************************************************************/

#include <Scope/Core/Trigger.h>
#include <math.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __TriggerPrivateData
{
  float level;
  int edge;
  uint32_t triggerIndex;
  IFloatStreamHandle stream;
  TriggerStrategy triggerStrategies[3];
  TriggerStrategy run;
  uint32_t activeChannelId;
  ChannelHandle* channels;
  size_t amountOfChannels;
  bool isTriggered;
} TriggerPrivateData;

/* Checks if the trigger criteria matches the current data */
static bool checkCurrentData(TriggerHandle self, float* triggerData);

/* Fetches the current trigger values from the stream */
static bool getTriggerData(TriggerHandle self, float* data);

/* Strategy for the continous trigger */
static bool triggerContinuous(TriggerHandle self, uint32_t index);

/* Strategy for the normal trigger */
static bool triggerNormal(TriggerHandle self, uint32_t index);

/* Strategy for the one shot trigger */
static bool triggerOneShot(TriggerHandle self, uint32_t index);

/* Checks the given configuration data for possible errors and return false
 * if errors were found */
static bool configSanityCheck(TriggerHandle self, TriggerConfiguration conf);

/* Returns the right trigger strategy */
static TriggerStrategy getTriggerStrategy(TriggerHandle self, TRIGGER_MODE mode);

/* Configures the trigger */
static void writeConfig(TriggerHandle self, TriggerConfiguration conf);

/******************************************************************************
 Private functions
******************************************************************************/
static bool checkCurrentData(TriggerHandle self, float* triggerData){

  const float dataCurrent = triggerData[CHANNEL_OLD_DATA];
  const float dataLast = triggerData[CHANNEL_CURRENT_DATA];
  const float triggerLevel = self->level;
  const int edge = (const int) dataCurrent > dataLast ? 1 : -1;

  if(dataCurrent == dataLast){
    return false;
  }
  
  if(edge != self->edge){
    return false;
  }
  
  /* Sort the two values to check if the triggerLevel is between them */
  const float levelLower = dataCurrent > dataLast ? dataLast : dataCurrent;
  const float levelHigher = dataCurrent > dataLast ? dataCurrent : dataLast;
  if(triggerLevel < levelLower || triggerLevel > levelHigher){
    return false;
  }

  return true;
}

static bool getTriggerData(TriggerHandle self, float* data){

  if(self->stream->length(self->stream) != 2){
    return false;
  }

  self->stream->read(self->stream, data, 2);

  return true;
}

static bool triggerContinuous(TriggerHandle self, const uint32_t index){
  return false;
}

static bool triggerNormal(TriggerHandle self, const uint32_t index){
  float triggerData[2];
  bool streamSuccessfull = getTriggerData(self, triggerData);
  
  if(streamSuccessfull == false){
    return false;
  }

  bool isTriggered = checkCurrentData(self, triggerData);

  /* If the trigger is already triggered, it shouldn't overwrite this status
   * But is should still be able to update the trigger index if a new trigger event occurred */
  if(isTriggered == true){
    self->triggerIndex = index;
  }
  if(self->isTriggered == true){
    isTriggered = true;
  }

  return isTriggered;
}

static bool triggerOneShot(TriggerHandle self, const uint32_t index){
  return false;
}

static bool configSanityCheck(TriggerHandle self, TriggerConfiguration conf){
  if((conf.mode != TRIGGER_NORMAL) 
      && (conf.mode != TRIGGER_CONTINUOUS) 
      && (conf.mode != TRIGGER_ONESHOT)){
    return false;
  }
  if((conf.edge != TRIGGER_EDGE_NEGATIVE)
      && (conf.edge != TRIGGER_EDGE_POSITIVE)){
    return false;
  }
  if(conf.channelId < 0){
    return false;
  }
  if(conf.channelId >= self->amountOfChannels){
    return false;
  }
  return true;
}

static TriggerStrategy getTriggerStrategy(TriggerHandle self, TRIGGER_MODE mode){
  return self->triggerStrategies[mode];
}

static void writeConfig(TriggerHandle self, TriggerConfiguration conf){
  self->level = conf.level;
  self->edge = conf.edge;
  self->stream = Channel_getTriggerDataStream(self->channels[conf.channelId]);
  self->run = getTriggerStrategy(self, conf.mode);
  self->activeChannelId = conf.channelId;
}

/******************************************************************************
 Public functions
******************************************************************************/
TriggerHandle Trigger_create(ChannelHandle* channels, size_t amountOfChannels){

  TriggerHandle self = malloc(sizeof(TriggerPrivateData));
  self->channels = malloc(sizeof(ChannelHandle) * amountOfChannels);
  self->channels = channels;
  self->amountOfChannels = amountOfChannels;
  /* Writing all strategies into a array to easier load it when the trigger 
     is reconfigured */
  self->triggerStrategies[TRIGGER_CONTINUOUS] = &triggerContinuous;
  self->triggerStrategies[TRIGGER_NORMAL] = &triggerNormal;
  self->triggerStrategies[TRIGGER_ONESHOT] = &triggerOneShot;
  self->activeChannelId = 0;
  self->isTriggered = false;
  
  self->run = &triggerContinuous;
  
  return self;
}

void Trigger_destroy(TriggerHandle self){
  free(self);
  self = NULL;
}

uint32_t Trigger_getTriggerIndex(TriggerHandle self){
    return self->triggerIndex;
}

bool Trigger_configure(TriggerHandle self, TriggerConfiguration conf){
    
    if(configSanityCheck(self, conf) == false){
      return false;
    }
    writeConfig(self, conf);
    return true;
}

bool Trigger_run(TriggerHandle self, const uint32_t timestamp){
  self->isTriggered = self->run(self, timestamp);
  return self->isTriggered;
}

bool Trigger_isTriggered(TriggerHandle self){
  return self->isTriggered;
}

void Trigger_release(TriggerHandle self){
  self->isTriggered = false;
  self->triggerIndex = 0;
}

uint32_t Trigger_getChannelId(TriggerHandle self){
  return self->activeChannelId;
}