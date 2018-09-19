/*!*****************************************************************************
 * @file         Trigger.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 * 
 ******************************************************************************/

#include <Scope/Trigger.h>
#include <math.h>

/* Define public data */
typedef struct __TriggerPrivateStruct
{
  float level;
  int sign;
  int edge;
  ssize_t triggerIndex;
  ChannelHandle channel;
  TriggerStrategy triggerStrategies[3];
} TriggerPrivateData;

/* Private functions */
static bool checkCurrentData(TriggerHandle self, float* triggerData){
  
  const float dataCurrent = triggerData[CHANNEL_CURRENT_DATA];
  const float dataLast = triggerData[CHANNEL_OLD_DATA];
  const float triggerLevel = self->level;
  const int edge = (const int) dataCurrent > dataLast ? 1 : -1;
  const int sign = (const int) copysign(1.0f, dataCurrent);
  
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

static bool triggerContinuous(TriggerHandle self, const int index){
  self->triggerIndex = -1;
  return false;
}

static bool triggerNormal(TriggerHandle self, const int index){
  
  ChannelHandle channel = self->channel;
  
  float triggerData[2];
  Channel_getTriggerData(channel, triggerData);

  const bool isTriggered = checkCurrentData(self, triggerData);
  
  if(isTriggered == true){
    self->triggerIndex = index;
  }
  
  return isTriggered;
}

static bool triggerOneShot(TriggerHandle self, const int index){
  return false;
}


/* Public functions */
TriggerHandle Trigger_create(){

  TriggerHandle self = malloc(sizeof(TriggerPrivateData));
  
  /* Writing all strategies into a array to easier load it when the trigger 
     is reconfigured */
  self->triggerStrategies[TRIGGER_CONTINUOUS] = &triggerContinuous;
  self->triggerStrategies[TRIGGER_NORMAL] = &triggerNormal;
  self->triggerStrategies[TRIGGER_ONESHOT] = &triggerOneShot;
  
  Trigger_run = &triggerContinuous;
  
  return self;
}

void Trigger_destroy(TriggerHandle self){
  free(self);
}

bool configSanityCheck(TriggerConfiguration conf){
  if((conf.mode != TRIGGER_NORMAL) \ 
      && (conf.mode != TRIGGER_CONTINUOUS) \ 
      && (conf.mode != TRIGGER_ONESHOT)){
    return false;
  }
  if((conf.edge != TRIGGER_EDGE_NEGATIVE) \
      && (conf.edge != TRIGGER_EDGE_POSITIVE)){
    return false;
  }
  if(conf.channel == NULL){
    return false;
  }
  return true;
}

TriggerStrategy getTriggerStrategy(TriggerHandle self, TRIGGER_MODE mode){
  return self->triggerStrategies[mode];
}

void writeConfig(TriggerHandle self, TriggerConfiguration conf){
  self->level = conf.level;
  self->edge = conf.edge;
  self->sign = (int) copysign(1.0f, conf.level);
  self->channel = conf.channel;
  Trigger_run = getTriggerStrategy(self, conf.mode);
}

int Trigger_getTriggerIndex(TriggerHandle self){
    return (int) self->triggerIndex;
}

bool Trigger_configure(TriggerHandle self, TriggerConfiguration conf){
    
    if(configSanityCheck(conf) == false){
      return false;
    }
    writeConfig(self, conf);
    return true;
}
