/*** Mailbox handling APIs ***/

/************************************************************************
*
*   This file contains the definitions of functions concerned with the
*   handling of mailboxes in Nucleus SE:
*
*   NUSE_Mailbox_Send()
*   NUSE_Mailbox_Receive()
*   NUSE_Mailbox_Reset()
*   NUSE_Mailbox_Information()
*   NUSE_Mailbox_Count()
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
*       NUSE_Mailbox_Send()
*
*   DESCRIPTION
*
*       Sends a single (type ADDR) message
*
*   INPUTS
*
*       NUSE_MAILBOX mailbox    index of mailbox to be used
*       ADDR *message           pointer to message to be sent
*       U8 suspend              task suspend option - must be NUSE_NO_SUSPEND or NUSE_SUSPEND
*                               depending upon the setting of NUSE_BLOCKING_ENABLE
*
*   RETURNS
*
*       NUSE_SUCCESS            message successfully sent/received
*       NUSE_MAILBOX_FULL       send failed as mailbox was in use
*       NUSE_INVALID_MAILBOX    specified mailbox index is invalid
*       NUSE_INVALID_POINTER    message pointer [message] is NULL
*       NUSE_INVALID_SUSPEND    task suspend option is not set to NUSE_NO_SUSPEND or NUSE_SUSPEND
*
*   OTHER OUTPUTS
*
*       (none)
*
*************************************************************************/


#if NUSE_MAILBOX_SEND || (NUSE_INCLUDE_EVERYTHING && (NUSE_MAILBOX_NUMBER != 0))

    STATUS NUSE_Mailbox_Send(NUSE_MAILBOX mailbox, ADDR *message, U8 suspend)
    {
        STATUS return_value;

        #if NUSE_API_PARAMETER_CHECKING
            if (mailbox >= NUSE_MAILBOX_NUMBER)
            {
                return NUSE_INVALID_MAILBOX;
            }

            if (message == NULL)
            {
                return NUSE_INVALID_POINTER;
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
                if (!NUSE_Mailbox_Status[mailbox])      /* mailbox empty */
                {
                    NUSE_Mailbox_Data[mailbox] = *message;
                    NUSE_Mailbox_Status[mailbox] = TRUE;
                    if (NUSE_Mailbox_Blocking_Count[mailbox] != 0)
                    {
                        U8 index;                       /* check whether a task is blocked */
                                                        /* on this mailbox */
                        NUSE_Mailbox_Blocking_Count[mailbox]--;
                        for (index=0; index<NUSE_TASK_NUMBER; index++)
                        {
                            if ((LONIB(NUSE_Task_Status[index]) == NUSE_MAILBOX_SUSPEND)
                                && (HINIB(NUSE_Task_Status[index]) == mailbox))
                            {
                                NUSE_Task_Blocking_Return[index] = NUSE_SUCCESS;
                                NUSE_Wake_Task(index);
                                break;
                            }
                        }
                    }
                    return_value = NUSE_SUCCESS;
                    suspend = NUSE_NO_SUSPEND;
                }
                else                                    /* mailbox full */
                {
                    if (suspend == NUSE_NO_SUSPEND)
                    {
                        return_value = NUSE_MAILBOX_FULL;
                    }
                    else
                    {                                   /* block task */
                        NUSE_Mailbox_Blocking_Count[mailbox]++;
                        NUSE_Suspend_Task(NUSE_Task_Active, (mailbox << 4) | NUSE_MAILBOX_SUSPEND);
                        return_value = NUSE_Task_Blocking_Return[NUSE_Task_Active];
                        if (return_value != NUSE_SUCCESS)
                        {
                            suspend = NUSE_NO_SUSPEND;
                        }
                    }
                }
            } while (suspend == NUSE_SUSPEND);
        #else
            if (NUSE_Mailbox_Status[mailbox])           /* mailbox full */
            {
                return_value = NUSE_MAILBOX_FULL;
            }
            else                                        /* mailbox empty */
            {
                NUSE_Mailbox_Data[mailbox] = *message;
                NUSE_Mailbox_Status[mailbox] = TRUE;
                return_value = NUSE_SUCCESS;
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
*       NUSE_Mailbox_Receive()
*
*   DESCRIPTION
*
*       Receives a single (type ADDR) message
*
*   INPUTS
*
*       NUSE_MAILBOX mailbox    index of mailbox to be used
*       ADDR *message           pointer to storage for message to be received
*       U8 suspend              task suspend option - must be NUSE_NO_SUSPEND or NUSE_SUSPEND
*                               depending upon the setting of NUSE_BLOCKING_ENABLE
*
*   RETURNS
*
*       NUSE_SUCCESS            message successfully sent/received
*       NUSE_MAILBOX_EMPTY      receive failed as no message was waiting
*       NUSE_INVALID_MAILBOX    specified mailbox index is invalid
*       NUSE_INVALID_POINTER    message pointer [message] is NULL
*       NUSE_INVALID_SUSPEND    task suspend option is not set to NUSE_NO_SUSPEND or NUSE_SUSPEND
*
*   OTHER OUTPUTS
*
*       ADDR *message           received message
*
*************************************************************************/


#if NUSE_MAILBOX_RECEIVE || (NUSE_INCLUDE_EVERYTHING && (NUSE_MAILBOX_NUMBER != 0))

    STATUS NUSE_Mailbox_Receive(NUSE_MAILBOX mailbox, ADDR *message, U8 suspend)
    {
        STATUS return_value;

        #if NUSE_API_PARAMETER_CHECKING
            if (mailbox >= NUSE_MAILBOX_NUMBER)
            {
                return NUSE_INVALID_MAILBOX;
            }

            if (message == NULL)
            {
                return NUSE_INVALID_POINTER;
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
                if (NUSE_Mailbox_Status[mailbox])           /* mailbox full */
                {
                    *message = NUSE_Mailbox_Data[mailbox];
                    NUSE_Mailbox_Status[mailbox] = FALSE;
                    if (NUSE_Mailbox_Blocking_Count[mailbox] != 0)
                    {
                        U8 index;                       /* check whether a task is blocked */
                                                        /* on this mailbox */
                        NUSE_Mailbox_Blocking_Count[mailbox]--;
                        for (index=0; index<NUSE_TASK_NUMBER; index++)
                        {
                            if ((LONIB(NUSE_Task_Status[index]) == NUSE_MAILBOX_SUSPEND)
                                && (HINIB(NUSE_Task_Status[index]) == mailbox))
                            {
                                NUSE_Task_Blocking_Return[index] = NUSE_SUCCESS;
                                NUSE_Wake_Task(index);
                                break;
                            }
                        }
                    }
                    return_value = NUSE_SUCCESS;
                    suspend = NUSE_NO_SUSPEND;
                }
                else                                        /* mailbox empty */
                {
                    if (suspend == NUSE_NO_SUSPEND)
                    {
                        return_value = NUSE_MAILBOX_EMPTY;
                    }
                    else
                    {                                       /* block task */
                        NUSE_Mailbox_Blocking_Count[mailbox]++;
                        NUSE_Suspend_Task(NUSE_Task_Active, (mailbox << 4) | NUSE_MAILBOX_SUSPEND);
                        return_value = NUSE_Task_Blocking_Return[NUSE_Task_Active];
                        if (return_value != NUSE_SUCCESS)
                        {
                            suspend = NUSE_NO_SUSPEND;
                        }
                    }
                }
            } while (suspend == NUSE_SUSPEND);
        #else
            if (!NUSE_Mailbox_Status[mailbox])          /* mailbox empty */
            {
                return_value = NUSE_MAILBOX_EMPTY;
            }
            else
            {                                           /* mailbox full */
                *message = NUSE_Mailbox_Data[mailbox];
                NUSE_Mailbox_Status[mailbox] = FALSE;
                return_value = NUSE_SUCCESS;
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
*       NUSE_Mailbox_Reset()
*
*   DESCRIPTION
*
*       Restores the specified mailbox to its initialized state
*       Any waiting message is lost
*       Uses the same initialization routine as on start-up - in nuse_init.c
*
*   INPUTS
*
*       NUSE_MAILBOX mailbox    index of mailbox to be reset
*
*   RETURNS
*
*       NUSE_SUCCESS            mailbox was successfully reset
*       NUSE_INVALID_MAILBOX    mailbox index was not valid
*
*   OTHER OUTPUTS
*
*       (none)
*
*************************************************************************/

#if NUSE_MAILBOX_RESET || (NUSE_INCLUDE_EVERYTHING && (NUSE_MAILBOX_NUMBER != 0))

    STATUS NUSE_Mailbox_Reset(NUSE_MAILBOX mailbox)
    {
        #if NUSE_API_PARAMETER_CHECKING
            if (mailbox >= NUSE_MAILBOX_NUMBER)
            {
                return NUSE_INVALID_MAILBOX;
            }
        #endif

        NUSE_CS_Enter();

        NUSE_Mailbox_Status[mailbox] = FALSE;

        #if NUSE_BLOCKING_ENABLE

            while (NUSE_Mailbox_Blocking_Count[mailbox] != 0)
            {
                U8 index;                       /* check whether any tasks are blocked */
                                                /* on this mailbox */

                for (index=0; index<NUSE_TASK_NUMBER; index++)
                {
                    if ((LONIB(NUSE_Task_Status[index]) == NUSE_MAILBOX_SUSPEND)
                        && (HINIB(NUSE_Task_Status[index]) == mailbox))
                    {
                        NUSE_Task_Blocking_Return[index] = NUSE_MAILBOX_WAS_RESET;
                        NUSE_Task_Status[index] = NUSE_READY;
                        break;
                    }
                }
                NUSE_Mailbox_Blocking_Count[mailbox]--;
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
*       NUSE_Mailbox_Information()
*
*   DESCRIPTION
*
*       Returns a number of items of data about a mailbox
*       Unsupported items are handled in nuse.h
*
*   INPUTS
*
*       NUSE_MAILBOX mailbox        index of mailbox about which information is required
*       U8 *message_present         pointer to storage for mailbox status
*       U8 *tasks_waiting           pointer to storage for number of waiting tasks
*       NUSE_TASK *first_task       pointer to storage for pointer to first waiting task
*
*   RETURNS
*
*       NUSE_SUCCESS                the mailbox status was successfully returned
*       NUSE_INVALID_MAILBOX        the mailbox index was not valid
*       NUSE_INVALID_POINTER        one or more of the pointer parameters was NULL
*
*   OTHER OUTPUTS
*
*       U8 *message_present         mailbox status: TRUE indicates full
*       U8 *tasks_waiting           number of waiting tasks [if used, otherwise 0]
*       NUSE_TASK *first_task       first waiting task [if used, otherwise 0]
*
*************************************************************************/

#if NUSE_MAILBOX_INFORMATION || (NUSE_INCLUDE_EVERYTHING && (NUSE_MAILBOX_NUMBER != 0))

    STATUS NUSE_Mailbox_Information(NUSE_MAILBOX mailbox, U8 *message_present, U8 *tasks_waiting, NUSE_TASK *first_task)
    {
        #if NUSE_API_PARAMETER_CHECKING
            if (mailbox >= NUSE_MAILBOX_NUMBER)
            {
                return NUSE_INVALID_MAILBOX;
            }

            if ((message_present == NULL) || (tasks_waiting == NULL) || (first_task == NULL))
            {
                return NUSE_INVALID_POINTER;
            }
        #endif

        *message_present = NUSE_Mailbox_Status[mailbox];

        #if NUSE_BLOCKING_ENABLE

            *tasks_waiting = NUSE_Mailbox_Blocking_Count[mailbox];
            if (NUSE_Mailbox_Blocking_Count[mailbox] != 0)
            {
                U8 index;

                for (index=0; index<NUSE_TASK_NUMBER; index++)
                {
                    if ((LONIB(NUSE_Task_Status[index]) == NUSE_MAILBOX_SUSPEND)
                        && (HINIB(NUSE_Task_Status[index]) == mailbox))
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

        return NUSE_SUCCESS;
    }

#endif


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Mailbox_Count()
*
*   DESCRIPTION
*
*       Returns the number of mailboxes in the system
*       Code is trivial, as the number of mailboxes is fixed in Nucleus SE,
*       so a constant is returned
*
*   INPUTS
*
*       (none)
*
*   RETURNS
*
*       U8 value        number of mailboxes in the system [Value: 0-15]
*
*   OTHER OUTPUTS
*
*       (none)
*
*************************************************************************/

#if NUSE_MAILBOX_COUNT || NUSE_INCLUDE_EVERYTHING

    U8 NUSE_Mailbox_Count(void)
    {
        return NUSE_MAILBOX_NUMBER;
    }

#endif
