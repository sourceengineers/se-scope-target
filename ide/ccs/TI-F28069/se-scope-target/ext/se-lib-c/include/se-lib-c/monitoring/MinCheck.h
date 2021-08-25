/*!****************************************************************************************************************************************
 * @file         MinCheck.h
 *
 * @copyright    © by Source Engineers GmbH, 2019
 *
 * @authors      Flurin Bühler <flurin.buehler@sourceengineers.com>
 *
 * @brief        TODO
 *
 *****************************************************************************************************************************************/

#ifndef MINCHECK_H_
#define MINCHECK_H_

#include <se-lib-c/util/checkable/ICheckable.h>
#include <se-lib-c/util/runnable/IRunnable.h>
#include <stdint.h>

/**
 * Declaration of the Template handle.
 */
typedef struct MinCheck_PrivateData* MinCheck_Handle;

/**
 * Creates a MinCheck instance.
 */
MinCheck_Handle MinCheck_create(float* valueToCheck, float tripLimit, uint16_t tripTimeMs, uint16_t callIntvervallMs);

/**
 * Destroys a MinCheck instance.
 */
void MinCheck_destroy(MinCheck_Handle me);

/**
 * Returns a pointer to the ICheckable.
 */ 
ICheckable* MinCheck_getICheckableInterface(MinCheck_Handle me);

/**
 * Returns a pointer to the IRunnable.
 */
IRunnable* MinCheck_getIRunnableInterface(MinCheck_Handle me);


#endif // MinCheck_H_
