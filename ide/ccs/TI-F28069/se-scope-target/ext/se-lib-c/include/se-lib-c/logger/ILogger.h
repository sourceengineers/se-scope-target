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


#include <se-lib-c/definition/SeLibCTypes.h>

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct ILoggerStruct* ILoggerHandle;
typedef enum severity {INFO, DEBUG, WARNING, ERROR} SEVERITY;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct ILoggerStruct{
    SeLibGenericReferece handle;

    /**
     * Adds log messages to the buffer
     * @param logger
     * @return
     */
 void (* log)(ILoggerHandle logger, SEVERITY severity, const char* msg);

} ILogger;



