/*** NUSE Semaphore handling APIs ***/

/************************************************************************
*
*   This file contains the definitions of functions concerned with the
*   handling of semaphores in Nucleus SE:
*
*   NUSE_Semaphore_Obtain()
*   NUSE_Semaphore_Release()
*   NUSE_Semaphore_Reset()
*   NUSE_Semaphore_Information()
*   NUSE_Semaphore_Count()
*
************************************************************************/

#include "nuse_config.h"
#include "nuse_codes.h"
#include "nuse_types.h"
#include "nuse_data.h"
#include "nuse_prototypes.h"


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Semaphore_Obtain()
*
*   DESCRIPTION
*
*       The specified semaphore is decremented, so long as its value was not initially 0
*
*   INPUTS
*
*       NUSE_SEMAPHORE semaphore    index of semaphore to be obtained
*       U8 suspend                  task suspend option - must be NUSE_NO_SUSPEND or NUSE_SUSPEND
*                                   depending upon the setting of NUSE_BLOCKING_ENABLE
*
*   RETURNS
*
*       NUSE_SUCCESS                the semaphore was successfully obtained
*       NUSE_UNAVAILABLE            the semaphore had the value 0 and could not, therefore, be obtained
*       NUSE_INVALID_SEMAPHORE      specified semaphore index is invalid
*       NUSE_INVALID_SUSPEND        task suspend option is not set to NUSE_NO_SUSPEND or NUSE_SUSPEND
*
*   OTHER OUTPUTS
*
*       (none)
*
*************************************************************************/

#if NUSE_SEMAPHORE_OBTAIN || (NUSE_INCLUDE_EVERYTHING && (NUSE_SEMPAHORE_NUMBER != 0))

    STATUS NUSE_Semaphore_Obtain(NUSE_SEMAPHORE semaphore, U8 suspend)
    {
        STATUS return_value;

        #if NUSE_API_PARAMETER_CHECKING
            if (semaphore >= NUSE_SEMAPHORE_NUMBER)
            {
                return NUSE_INVALID_SEMAPHORE;
            }

            #if NUSE_BLOCKING_ENABLE
                if ((suspend != NUSE_NO_SUSPEND) && (suspend != NUSE_SUSPEND))
                {
                    return NUSE_INVALID_SUSPEND;
                }
            #else
                if (suspend != NUSE_NO_SUSPEND)
                {
                    return NUSE_INVALID_SUSPEND;
                }
            #endif
        #endif

        NUSE_CS_Enter();

        #if NUSE_BLOCKING_ENABLE
            do
            {
                if (NUSE_Semaphore_Counter[semaphore] != 0)     /* semaphore available */
                {
                    NUSE_Semaphore_Counter[semaphore]--;
                    return_value = NUSE_SUCCESS;
                    suspend = NUSE_NO_SUSPEND;
                }
                else                                            /* semaphore unavailable */
                {
                    if (suspend == NUSE_NO_SUSPEND)
                    {
                        return_value = NUSE_UNAVAILABLE;
                    }
                    else
                    {                                   /* block task */
                        NUSE_Semaphore_Blocking_Count[semaphore]++;
                        NUSE_Suspend_Task(NUSE_Task_Active, (semaphore << 4) | NUSE_SEMAPHORE_SUSPEND);
                        return_value = NUSE_Task_Blocking_Return[NUSE_Task_Active];
                        if (return_value != NUSE_SUCCESS)
                        {
                            suspend = NUSE_NO_SUSPEND;
                        }
                    }
                }
            } while (suspend == NUSE_SUSPEND);
        #else
            if (NUSE_Semaphore_Counter[semaphore] != 0)         /* semaphore available */
            {
                NUSE_Semaphore_Counter[semaphore]--;
                return_value = NUSE_SUCCESS;
            }
            else                                                /* semaphore unavailable */
            {
                return_value = NUSE_UNAVAILABLE;
            }
        #endif

        NUSE_CS_Exit();

        return return_value;
    }

#endif


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Semaphore_Release()
*
*   DESCRIPTION
*
*       The specified semaphore is incremented, so long as its value was not initially 255
*
*   INPUTS
*
*       NUSE_SEMAPHORE semaphore    index of semaphore to be released
*
*   RETURNS
*
*       NUSE_SUCCESS                the semaphore was successfully obtained
*       NUSE_UNAVAILABLE            the semaphore had the value 255 and could not, therefore, be released
*       NUSE_INVALID_SEMAPHORE      specified sempahore index is invalid
*
*   OTHER OUTPUTS
*
*       (none)
*
*************************************************************************/

#if NUSE_SEMAPHORE_RELEASE || (NUSE_INCLUDE_EVERYTHING && (NUSE_SEMPAHORE_NUMBER != 0))

    STATUS NUSE_Semaphore_Release(NUSE_SEMAPHORE semaphore)
    {
        STATUS return_value;

        #if NUSE_API_PARAMETER_CHECKING
            if (semaphore >= NUSE_SEMAPHORE_NUMBER)
            {
                return NUSE_INVALID_SEMAPHORE;
            }
        #endif

        NUSE_CS_Enter();

        if (NUSE_Semaphore_Counter[semaphore] < 255)
        {
            NUSE_Semaphore_Counter[semaphore]++;
            return_value = NUSE_SUCCESS;

            #if NUSE_BLOCKING_ENABLE

                if (NUSE_Semaphore_Blocking_Count[semaphore] != 0)
                {
                    U8 index;                       /* check whether a task is blocked */
                                                    /* on this semaphore */

                    NUSE_Semaphore_Blocking_Count[semaphore]--;
                    for (index=0; index<NUSE_TASK_NUMBER; index++)
                    {
                        if ((LONIB(NUSE_Task_Status[index]) == NUSE_SEMAPHORE_SUSPEND)
                            && (HINIB(NUSE_Task_Status[index]) == semaphore))
                        {
                            NUSE_Task_Blocking_Return[index] = NUSE_SUCCESS;
                            NUSE_Wake_Task(index);
                            break;
                        }
                    }
                }

            #endif
        }
        else
        {
            return_value = NUSE_UNAVAILABLE;
        }

        NUSE_CS_Exit();

        return return_value;
    }

#endif


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Semaphore_Reset()
*
*   DESCRIPTION
*
*       Restores the specified sempahore to its initialized state
*
*   INPUTS
*
*       NUSE_SEMPAHORE semaphore    index of semaphore to be reset
*
*   RETURNS
*
*       NUSE_SUCCESS                semaphore was successfully reset
*       NUSE_INVALID_SEMAPHORE      semaphore index was not valid
*
*   OTHER OUTPUTS
*
*       (none)
*
*************************************************************************/

#if NUSE_SEMAPHORE_RESET || (NUSE_INCLUDE_EVERYTHING && (NUSE_SEMPAHORE_NUMBER != 0))

    STATUS NUSE_Semaphore_Reset(NUSE_SEMAPHORE semaphore, U8 initial_count)
    {
        #if NUSE_API_PARAMETER_CHECKING
            if (semaphore >= NUSE_SEMAPHORE_NUMBER)
            {
                return NUSE_INVALID_SEMAPHORE;
            }
        #endif

        NUSE_CS_Enter();

        NUSE_Semaphore_Counter[semaphore] = initial_count;

        #if NUSE_BLOCKING_ENABLE

            while (NUSE_Semaphore_Blocking_Count[semaphore] != 0)
            {
                U8 index;                       /* check whether any tasks are blocked */
                                                /* on this semaphore */

                for (index=0; index<NUSE_TASK_NUMBER; index++)
                {
                    if ((LONIB(NUSE_Task_Status[index]) == NUSE_SEMAPHORE_SUSPEND)
                        && (HINIB(NUSE_Task_Status[index]) == semaphore))
                    {
                        NUSE_Task_Blocking_Return[index] = NUSE_SEMAPHORE_WAS_RESET;
                        NUSE_Task_Status[index] = NUSE_READY;
                        break;
                    }
                }
                NUSE_Semaphore_Blocking_Count[semaphore]--;
            }

            #if NUSE_SCHEDULER_TYPE == NUSE_PRIORITY_SCHEDULER
                NUSE_Reschedule(NUSE_NO_TASK);
            #endif

        #endif

        NUSE_CS_Exit();

        return NUSE_SUCCESS;
    }

#endif

/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Semaphore_Information()
*
*   DESCRIPTION
*
*       Returns a number of items of data about a semaphore
*       Unsupported items are handled in nuse.h
*
*   INPUTS
*
*       NUSE_SEMAPHORE semaphore    index of semaphore about which information is required
*       U8 *current_count           pointer to storage for the current semaphore count value
*       U8 *tasks_waiting           pointer to storage for number of waiting tasks [only used if blocking enable]
*       NUSE_TASK *first_task       pointer to storage for pointer to first waiting task [only used if blocking enable]
*
*   RETURNS
*
*       NUSE_SUCCESS                the semaphore status was successfully returned
*       NUSE_INVALID_SEMAPHORE      the semaphore index was not valid
*       NUSE_INVALID_POINTER        one or more of the pointer parameters was NULL
*
*   OTHER OUTPUTS
*
*       U8 *current_count           current semaphore count value
*       U8 *tasks_waiting           number of waiting tasks [if used, otherwise 0]
*       NUSE_TASK *first_task       first waiting task [if used, otherwise 0]
*
*************************************************************************/

#if NUSE_SEMAPHORE_INFORMATION || (NUSE_INCLUDE_EVERYTHING && (NUSE_SEMPAHORE_NUMBER != 0))

    STATUS NUSE_Semaphore_Information(NUSE_SEMAPHORE semaphore, U8 *current_count, U8 *tasks_waiting, NUSE_TASK *first_task)
    {
        #if NUSE_API_PARAMETER_CHECKING
            if (semaphore >= NUSE_SEMAPHORE_NUMBER)
            {
                return NUSE_INVALID_SEMAPHORE;
            }

            if ((current_count == NULL) || (tasks_waiting == NULL) || (first_task == NULL))
            {
                return NUSE_INVALID_POINTER;
            }
        #endif

        NUSE_CS_Enter();

        *current_count = NUSE_Semaphore_Counter[semaphore];

        #if NUSE_BLOCKING_ENABLE

            *tasks_waiting = NUSE_Semaphore_Blocking_Count[semaphore];
            if (NUSE_Semaphore_Blocking_Count[semaphore] != 0)
            {
                U8 index;

                for (index=0; index<NUSE_TASK_NUMBER; index++)
                {
                    if ((LONIB(NUSE_Task_Status[index]) == NUSE_SEMAPHORE_SUSPEND)
                        && (HINIB(NUSE_Task_Status[index]) == semaphore))
                    {
                        *first_task = index;
                        break;
                    }
                }
            }
            else
            {
                *first_task = 0;
            }

        #else

            *tasks_waiting = 0;
            *first_task = 0;

        #endif

        NUSE_CS_Exit();

        return NUSE_SUCCESS;
    }

#endif


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Semaphore_Count()
*
*   DESCRIPTION
*
*       Returns the number of semaphores in the system
*       Code is trivial, as the number of semaphores is fixed in Nucleus SE,
*       so a constant is returned
*
*   INPUTS
*
*       (none)
*
*   RETURNS
*
*       U8 value        number of semaphores in the system [Value: 0-15]
*
*   OTHER OUTPUTS
*
*       (none)
*
*************************************************************************/

#if NUSE_SEMAPHORE_COUNT || NUSE_INCLUDE_EVERYTHING

    U8 NUSE_Semaphore_Count(void)
    {
        return NUSE_SEMAPHORE_NUMBER;
    }

#endif
