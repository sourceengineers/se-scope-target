/*!*****************************************************************************
 * @file         ${HEADER_FILENAME}
 *
 * @copyright    Copyright (c) 2019 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Benjamin Rupp  benjamin.rupp@sourceengineers.com
 *
 * @brief        
 *
 *
 ******************************************************************************/

#ifndef SE_SCOPE_LOGGER_H
#define SE_SCOPE_LOGGER_H

#include <se-lib-c/logger/ILogger.h>
#include <se-lib-c/util/observer/IObserver.h>
#include <stdbool.h>
#include <stddef.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __LoggerPrivateData* LoggerHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param 
 * @return
 */
LoggerHandle Logger_create(size_t bufferSize);

/**
 * Log of messages
 * @param self
 * @return
 */
void Logger_log(LoggerHandle self, SEVERITY severity, const char* msg );

/**
 * Returns the ILogger interface
 * @param LoggerHandle
 * @return ILoggerHandle
 */
ILoggerHandle Logger_getILogger(LoggerHandle self);

/**
 * Reallocates more memory in case there is a long string
 * @param mem
 */
bool Logger_reallocateMemory(LoggerHandle  self, size_t mem);

/**
 * Returns the messagebuffer
 * @param self
 * @return char*
 */
char* Logger_getBuffer(LoggerHandle self);

/**
 * Returns the buffered byte stream
 * @param self
 * @return char*
 */
char* Logger_getBufferedByteStream(LoggerHandle self);

/**
 * Attatches the observer which has to be called to execute a pack event
 * @param self
 * @param observer
 */
void Logger_attachBufferObserver(LoggerHandle self, IObserverHandle observer);

/**
 * Calculates the buffere size for the byte stream
 * @param
 * @param calculated buffer size
 */
size_t Logger_calculateBufferSize();

/**
 * Destructor
 * @param
 * @return
 */
 void LoggerDestroy(LoggerHandle self);

#endif //SE_SCOPE_LOGGER_H
