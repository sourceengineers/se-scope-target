/*!****************************************************************************************************************************************
 * @file         RunnableList.c
 *
 * @copyright    © by Source Engineers GmbH, 2019
 *
 * @authors      Flurin Bühler <flurin.buehler@sourceengineers.com>
 *
 * @brief        TODO
 *
 *****************************************************************************************************************************************/
#include <se-lib-c/util/runnable/RunnableList.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct RunnableListElement* RunnableListElement_Handle;

typedef struct RunnableListElement
{
    IRunnable* runnable;
    RunnableListElement_Handle next;
}ListElement;

typedef struct RunnableList_PrivateData
{
	IRunnable parent;
    RunnableListElement_Handle first;
	uint16_t numberOfListElements;
} PrivateData;


/**
 * The implementation of the Parent methods.
 * @{
 */ 
static void run(IRunnableHandle handle);
/** @} */


RunnableList_Handle RunnableList_create(void)
{
    PrivateData* me = malloc(sizeof(PrivateData));
    assert(me != NULL);

    // initialize interface
    me->parent.handle = me;
    me->parent.run = &run;

    // initialize private variables
    me->first = NULL;

    return me;
}

void RunnableList_destroy(RunnableList_Handle me)
{
    if(me->first != NULL)
    {
        RunnableListElement_Handle element = (RunnableListElement_Handle)me->first;
        while(element->next != NULL)
        {
            RunnableListElement_Handle elementToFree = element;
            element = element->next;
            free(elementToFree);
        }
    }

    free(me);
}


IRunnable* RunnableList_getIRunnableInterface(RunnableList_Handle me)
{
    assert(me != NULL);
    return &me->parent;
}

void RunnableList_add(RunnableList_Handle me, IRunnable* runnable)
{
    assert(runnable);
    assert(me);

    if(me->first == NULL)
    {
        me->first = (RunnableListElement_Handle)malloc(sizeof(ListElement));
        assert(me->first);
        me->first->runnable = runnable;
        me->first->next = NULL;
    }
    else
    {
        RunnableListElement_Handle element = me->first;
        while(element->next != NULL)
        {
            element = element->next;
        }
        element->next =  (RunnableListElement_Handle)malloc(sizeof(ListElement));
        assert(element->next);
        element = element->next;
        element->runnable = runnable;
        element->next = NULL;
    }
}


static void run(IRunnableHandle handle)
{
    PrivateData* me = (PrivateData*)handle->handle;
    assert(me != NULL);

    RunnableListElement_Handle element = me->first;
    while(element->next != NULL)
    {
        assert(element->runnable);
        element->runnable->run(element->runnable->handle);
        element = element->next;
    }
    // run last element
    assert(element->runnable);
    element->runnable->run(element->runnable->handle);
}
