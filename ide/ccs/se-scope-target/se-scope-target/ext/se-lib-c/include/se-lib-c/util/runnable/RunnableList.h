/*!****************************************************************************************************************************************
 * @file         RunnableList.h
 *
 * @copyright    © by Source Engineers GmbH, 2019
 *
 * @authors      Flurin Bühler <flurin.buehler@sourceengineers.com>
 *
 * @brief        TODO
 *
 *****************************************************************************************************************************************/

#ifndef RUNNABLELIST_H_
#define RUNNABLELIST_H_

#include <se-lib-c/util/runnable/IRunnable.h>

/**
 * Declaration of the Template handle.
 */
typedef struct RunnableList_PrivateData* RunnableList_Handle;

/**
 * Creates a RunnableList instance.
 */
RunnableList_Handle RunnableList_create(void);

/**
 * Destroys a RunnableList instance.
 */
void RunnableList_destroy(RunnableList_Handle me);

/**
 * Add a runnable to the list
 */
void RunnableList_add(RunnableList_Handle me, IRunnable*  runnable);


/**
 * Returns a pointer to the IRunnable.
 */ 
IRunnable* RunnableList_getIRunnableInterface(RunnableList_Handle me);


#endif // RUNNABLELIST_H_
