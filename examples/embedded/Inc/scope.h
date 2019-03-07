//
// Created by schuepbs on 01.02.19.
//

#ifndef NUCLEO_SCOPE_H
#define NUCLEO_SCOPE_H

#include <SE-Scope.h>
#include <Scope/Communication/Interfaces/UartJson.h>
#include <Scope/Serialisation/JsonParser/JsonUnpacker.h>
#include <Scope/Serialisation/JsonParser/JsonPacker.h>

extern AddressStorageHandle addressStorage;
extern size_t outputBufferSize;
extern UartJsonHandle uartJson;

void scope_init(UartTransmitCallback callback, uint32_t* timestamp);

void scope_run();


#endif //NUCLEO_SCOPE_H
