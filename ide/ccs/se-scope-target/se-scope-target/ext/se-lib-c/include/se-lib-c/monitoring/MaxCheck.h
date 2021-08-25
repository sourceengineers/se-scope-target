/*!****************************************************************************************************************************************
 * @file         MaxCheck.h
 *
 * @copyright    © by Source Engineers GmbH, 2019
 *
 * @authors      Flurin Bühler <flurin.buehler@sourceengineers.com>
 *
 * @brief        TODO
 *
 *****************************************************************************************************************************************/

#ifndef MAXCHECK_H_
#define MAXCHECK_H_

#include <se-lib-c/util/checkable/ICheckable.h>
#include <se-lib-c/util/runnable/IRunnable.h>
#include <stdint.h>

/**
 * Declaration of the Template handle.
 */
typedef struct MaxCheck_PrivateData* MaxCheck_Handle;

/**
 * Creates a MaxCheck instance.
 */
MaxCheck_Handle MaxCheck_create(float* valueToCheck, float tripLimit, uint16_t tripTimeMs, uint16_t callIntvervallMs);

/**
 * Destroys a MaxCheck instance.
 */
void MaxCheck_destroy(MaxCheck_Handle me);

/**
 * Returns a pointer to the ICheckable.
 */ 
ICheckable* MaxCheck_getICheckableInterface(MaxCheck_Handle me);

/**
 * Returns a pointer to the IRunnable.
 */
IRunnable* MaxCheck_getIRunnableInterface(MaxCheck_Handle me);


#endif // MAXCHECK_H_
