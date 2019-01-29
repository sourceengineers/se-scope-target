/*!****************************************************************************************************************************************
 * @file         UartJson.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Communication/Interfaces/UartJson.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __UartJsonPrivateData{

    ICommunicator communicator;

    TransmitCallback callback;
    IByteStreamHandle input;
    IByteStreamHandle output;

} UartJsonPrivateData;

/******************************************************************************
 Private functions
******************************************************************************/
void runRx(ICommunicatorHandle communicator){
    UartJsonHandle self = (UartJsonHandle) communicator->handle;


}

void runTx(ICommunicatorHandle communicator){
    UartJsonHandle self = (UartJsonHandle) communicator->handle;

}

/******************************************************************************
 Public functions
******************************************************************************/
UartJsonHandle UartJson_create(TransmitCallback callback, IByteStreamHandle input, IByteStreamHandle output){

    UartJsonHandle self = malloc(sizeof(UartJsonPrivateData));

    self->communicator.runRx = &runRx;
    self->communicator.runTx = &runTx;

    self->callback = callback;
    self->input = input;
    self->output = output;

    return self;
}

ICommunicatorHandle UartJson_getCommunicator(UartJsonHandle self){
    return &self->communicator;
}

void UartJson_destroy(UartJsonHandle self){

    free(self);
    self = NULL;
}
