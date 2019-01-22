/*!****************************************************************************************************************************************
 * @file         Serializer.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Serialisation/Serializer.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __SerializerPrivateData {

    IRunnableHandle packer;
    IRunnableHandle unpacker;

} SerializerPrivateData;


/******************************************************************************
 Private functions
******************************************************************************/
void runRx(IRxTxRunnableHandle runnable){

}

void runTx(IRxTxRunnableHandle runnable){
}
/******************************************************************************
 Public functions
******************************************************************************/
SerializerHandle Serializer_create(IRunnableHandle packer, IRunnableHandle unpacker);




void Serializer_destroy(SerializerHandle self);