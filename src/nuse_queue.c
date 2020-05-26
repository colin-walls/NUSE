/*** Queue handling APIs ***/

/************************************************************************
*
*   This file contains the definitions of functions concerned with the
*   handling of queues in Nucleus SE:
*
*   NUSE_Queue_Send()
*   NUSE_Queue_Receive()
*   NUSE_Queue_Jam()
*   NUSE_Queue_Reset()
*   NUSE_Queue_Information()
*   NUSE_Queue_Count()
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
*       NUSE_Queue_Send()
*
*   DESCRIPTION
*
*       Sends a single (type ADDR) message to the queue
*
*   INPUTS
*
*       NUSE_QUEUE queue        index of queue to be used
*       ADDR *message           pointer to the message to be sent
*       U8 suspend              task suspend option - must be NUSE_NO_SUSPEND or NUSE_SUSPEND
*                               depending upon the setting of NUSE_BLOCKING_ENABLE
*
*   RETURNS
*
*       NUSE_SUCCESS            message successfully sent/received
*       NUSE_QUEUE_FULL         send failed as there was no room in the queue
*       NUSE_INVALID_QUEUE      specified queue index is invalid
*       NUSE_INVALID_POINTER    message pointer [message] is NULL
*       NUSE_INVALID_SUSPEND    task suspend option is not set to NUSE_NO_SUSPEND or NUSE_SUSPEND
*
*   OTHER OUTPUTS
*
*       (none)
*
*************************************************************************/

#if NUSE_QUEUE_SEND || (NUSE_INCLUDE_EVERYTHING && (NUSE_QUEUE_NUMBER != 0))

    STATUS NUSE_Queue_Send(NUSE_QUEUE queue, ADDR *message, U8 suspend)
    {
        STATUS return_value;

        #if NUSE_API_PARAMETER_CHECKING
            if (queue >= NUSE_QUEUE_NUMBER)
            {
                return NUSE_INVALID_QUEUE;
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
                if (NUSE_Queue_Items[queue] == NUSE_Queue_Size[queue])      /* queue full */
                {
                    if (suspend == NUSE_NO_SUSPEND)
                    {
                        return_value = NUSE_QUEUE_FULL;
                    }
                    else
                    {                                                       /* block task */
                        NUSE_Queue_Blocking_Count[queue]++;
                        NUSE_Suspend_Task(NUSE_Task_Active, (queue << 4) | NUSE_QUEUE_SUSPEND);
                        return_value = NUSE_Task_Blocking_Return[NUSE_Task_Active];
                        if (return_value != NUSE_SUCCESS)
                        {
                            suspend = NUSE_NO_SUSPEND;
                        }
                    }
                }
                else
                {                                                           /* queue element available */
                    NUSE_Queue_Data[queue][NUSE_Queue_Head[queue]++] = *message;
                    if (NUSE_Queue_Head[queue] == NUSE_Queue_Size[queue])
                    {
                        NUSE_Queue_Head[queue] = 0;
                    }
                    NUSE_Queue_Items[queue]++;
                    if (NUSE_Queue_Blocking_Count[queue] != 0)
                    {
                        U8 index;                                           /* check whether a task is blocked */
                                                                            /* on this queue */

                        NUSE_Queue_Blocking_Count[queue]--;
                        for (index=0; index<NUSE_TASK_NUMBER; index++)
                        {
                            if ((LONIB(NUSE_Task_Status[index]) == NUSE_QUEUE_SUSPEND)
                                && (HINIB(NUSE_Task_Status[index]) == queue))
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
            } while (suspend == NUSE_SUSPEND);
        #else
            if (NUSE_Queue_Items[queue] == NUSE_Queue_Size[queue])          /* queue full */
            {
                return_value = NUSE_QUEUE_FULL;
            }
            else                                                            /* queue element available */
            {
                NUSE_Queue_Data[queue][NUSE_Queue_Head[queue]++] = *message;
                if (NUSE_Queue_Head[queue] == NUSE_Queue_Size[queue])
                {
                    NUSE_Queue_Head[queue] = 0;
                }
                NUSE_Queue_Items[queue]++;
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
*       NUSE_Queue_Receive()
*
*   DESCRIPTION
*
*       Receives a single (type ADDR) message from the queue
*
*   INPUTS
*
*       NUSE_QUEUE queue        index of queue to be used
*       ADDR *message           pointer to storage for message to be received
*       U8 suspend              task suspend option - must be NUSE_NO_SUSPEND or NUSE_SUSPEND
*                               depending upon the setting of NUSE_BLOCKING_ENABLE
*
*   RETURNS
*
*       NUSE_SUCCESS            message successfully sent/received
*       NUSE_QUEUE_EMPTY        receive failed as no message was waiting
*       NUSE_INVALID_QUEUE      specified queue index is invalid
*       NUSE_INVALID_POINTER    message pointer [message] is NULL
*       NUSE_INVALID_SUSPEND    task suspend option is not set to NUSE_NO_SUSPEND or NUSE_SUSPEND
*
*   OTHER OUTPUTS
*
*       ADDR *message           received message
*
*************************************************************************/

#if NUSE_QUEUE_RECEIVE || (NUSE_INCLUDE_EVERYTHING && (NUSE_QUEUE_NUMBER != 0))

    STATUS NUSE_Queue_Receive(NUSE_QUEUE queue, ADDR *message, U8 suspend)
    {
        STATUS return_value;

        #if NUSE_API_PARAMETER_CHECKING
            if (queue >= NUSE_QUEUE_NUMBER)
            {
                return NUSE_INVALID_QUEUE;
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
                if (NUSE_Queue_Items[queue] == 0)                           /* queue empty */
                {
                    if (suspend == NUSE_NO_SUSPEND)
                    {
                        return_value = NUSE_QUEUE_EMPTY;
                    }
                    else
                    {                                                       /* block task */
                        NUSE_Queue_Blocking_Count[queue]++;
                        NUSE_Suspend_Task(NUSE_Task_Active, (queue << 4) | NUSE_QUEUE_SUSPEND);
                        return_value = NUSE_Task_Blocking_Return[NUSE_Task_Active];
                        if (return_value != NUSE_SUCCESS)
                        {
                            suspend = NUSE_NO_SUSPEND;
                        }
                    }
                }
                else
                {                                                           /* message available */
                    *message = NUSE_Queue_Data[queue][NUSE_Queue_Tail[queue]++];
                    if (NUSE_Queue_Tail[queue] == NUSE_Queue_Size[queue])
                    {
                        NUSE_Queue_Tail[queue] = 0;
                    }
                    NUSE_Queue_Items[queue]--;
                    if (NUSE_Queue_Blocking_Count[queue] != 0)
                    {
                        U8 index;                                           /* check whether a task is blocked */
                                                                            /* on this queue */

                        NUSE_Queue_Blocking_Count[queue]--;
                        for (index=0; index<NUSE_TASK_NUMBER; index++)
                        {
                            if ((LONIB(NUSE_Task_Status[index]) == NUSE_QUEUE_SUSPEND)
                                && (HINIB(NUSE_Task_Status[index]) == queue))
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
            } while (suspend == NUSE_SUSPEND);
        #else
            if (NUSE_Queue_Items[queue] == 0)                               /* queue empty */
            {
                return_value = NUSE_QUEUE_EMPTY;
            }
            else
            {                                                               /* message available */
                *message = NUSE_Queue_Data[queue][NUSE_Queue_Tail[queue]++];
                if (NUSE_Queue_Tail[queue] == NUSE_Queue_Size[queue])
                {
                    NUSE_Queue_Tail[queue] = 0;
                }
                NUSE_Queue_Items[queue]--;
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
*       NUSE_Queue_Jam()
*
*   DESCRIPTION
*
*       Sends a single (type ADDR) message to the front of the queue [also known as "jamming" a message]
*
*   INPUTS
*
*       NUSE_QUEUE queue        index of queue to be used
*       ADDR *message           pointer to the message to be sent
*       U8 suspend              task suspend option - must be NUSE_NO_SUSPEND or NUSE_SUSPEND
*                               depending upon the setting of NUSE_BLOCKING_ENABLE
*
*   RETURNS
*
*       NUSE_SUCCESS            message successfully sent/received
*       NUSE_QUEUE_FULL         send failed as there was no room in the queue
*       NUSE_INVALID_QUEUE      specified queue index is invalid
*       NUSE_INVALID_POINTER    message pointer [message] is NULL
*       NUSE_INVALID_SUSPEND    task suspend option is not set to NUSE_NO_SUSPEND or NUSE_SUSPEND
*
*   OTHER OUTPUTS
*
*       (none)
*
*************************************************************************/

#if NUSE_QUEUE_JAM || (NUSE_INCLUDE_EVERYTHING && (NUSE_QUEUE_NUMBER != 0))

    STATUS NUSE_Queue_Jam(NUSE_QUEUE queue, ADDR *message, U8 suspend)
    {
        STATUS return_value;

        #if NUSE_API_PARAMETER_CHECKING
            if (queue >= NUSE_QUEUE_NUMBER)
            {
                return NUSE_INVALID_QUEUE;
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
                if (NUSE_Queue_Items[queue] == NUSE_Queue_Size[queue])      /* queue full */
                {
                    if (suspend == NUSE_NO_SUSPEND)
                    {
                        return_value = NUSE_QUEUE_FULL;
                    }
                    else
                    {                                                       /* block task */
                        NUSE_Queue_Blocking_Count[queue]++;
                        NUSE_Suspend_Task(NUSE_Task_Active, (queue << 4) | NUSE_QUEUE_SUSPEND);
                        return_value = NUSE_Task_Blocking_Return[NUSE_Task_Active];
                        if (return_value != NUSE_SUCCESS)
                        {
                            suspend = NUSE_NO_SUSPEND;
                        }
                    }
                }
                else
                {                                                           /* queue element available */
                    if (NUSE_Queue_Tail[queue] == 0)
                    {
                        NUSE_Queue_Tail[queue] = NUSE_Queue_Size[queue] - 1;
                    }
                    else
                    {
                        NUSE_Queue_Tail[queue]--;
                    }

                    NUSE_Queue_Data[queue][NUSE_Queue_Tail[queue]] = *message;
                    NUSE_Queue_Items[queue]++;
                    if (NUSE_Queue_Blocking_Count[queue] != 0)
                    {
                        U8 index;                                           /* check whether a task is blocked */
                                                                            /* on this queue */

                        NUSE_Queue_Blocking_Count[queue]--;
                        for (index=0; index<NUSE_TASK_NUMBER; index++)
                        {
                            if ((LONIB(NUSE_Task_Status[index]) == NUSE_QUEUE_SUSPEND)
                                && (HINIB(NUSE_Task_Status[index]) == queue))
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
            } while (suspend == NUSE_SUSPEND);
        #else
            if (NUSE_Queue_Items[queue] == NUSE_Queue_Size[queue])          /* queue full */
            {
                return_value = NUSE_QUEUE_FULL;
            }
            else                                                            /* queue element available */
            {
                if (NUSE_Queue_Tail[queue] == 0)
                {
                    NUSE_Queue_Tail[queue] = NUSE_Queue_Size[queue] - 1;
                }
                else
                {
                    NUSE_Queue_Tail[queue]--;
                }

                NUSE_Queue_Data[queue][NUSE_Queue_Tail[queue]] = *message;
                NUSE_Queue_Items[queue]++;
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
*       NUSE_Queue_Reset()
*
*   DESCRIPTION
*
*       Restores the specified queue to its initialized state
*       Any waiting messages are lost
*       Uses the same initialization routine as on start-up - in nuse_init.c
*
*   INPUTS
*
*       NUSE_QUEUE queue    index of queue to be reset
*
*   RETURNS
*
*       NUSE_SUCCESS        queue was successfully reset
*       NUSE_INVALID_QUEUE  queue index was not valid
*
*   OTHER OUTPUTS
*
*       (none)
*
*************************************************************************/

#if NUSE_QUEUE_RESET || (NUSE_INCLUDE_EVERYTHING && (NUSE_QUEUE_NUMBER != 0))

    STATUS NUSE_Queue_Reset(NUSE_QUEUE queue)
    {
        #if NUSE_API_PARAMETER_CHECKING
            if (queue >= NUSE_QUEUE_NUMBER)
            {
                return NUSE_INVALID_QUEUE;
            }
        #endif

        NUSE_CS_Enter();

        NUSE_Queue_Head[queue] = 0;
        NUSE_Queue_Tail[queue] = 0;
        NUSE_Queue_Items[queue] = 0;

        #if NUSE_BLOCKING_ENABLE

            while (NUSE_Queue_Blocking_Count[queue] != 0)
            {
                U8 index;                                           /* check whether any tasks are blocked */
                                                                    /* on this queue */

                for (index=0; index<NUSE_TASK_NUMBER; index++)
                {
                    if ((LONIB(NUSE_Task_Status[index]) == NUSE_QUEUE_SUSPEND)
                        && (HINIB(NUSE_Task_Status[index]) == queue))
                    {
                        NUSE_Task_Blocking_Return[index] = NUSE_QUEUE_WAS_RESET;
                        NUSE_Task_Status[index] = NUSE_READY;
                        break;
                    }
                }
                NUSE_Queue_Blocking_Count[queue]--;
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
*       NUSE_Queue_Information()
*
*   DESCRIPTION
*
*       Returns a number of items of data about a queue
*       Unsupported items are handled in nuse.h
*
*   INPUTS
*
*       NUSE_QUEUE queue            index of queue about which information is required
*       ADDR *start_address         pointer to storage for the start address of the queue
*       U8 *queue_size              pointer to storage for the size of the queue
*       U8 *available               pointer to storage for the number of free queue entries
*       U8 *messages                pointer to storage for the number of messages in the queue
*       U8 *tasks_waiting           pointer to storage for number of waiting tasks [if used]
*       NUSE_TASK *first_task        pointer to storage for pointer to first waiting task [if used]
*
*   RETURNS
*
*       NUSE_SUCCESS                the queue status was successfully returned
*       NUSE_INVALID_QUEUE          the queue index was not valid
*       NUSE_INVALID_POINTER        one or more of the pointer parameters was NULL
*
*   OTHER OUTPUTS
*
*       ADDR *start_address         start address of the queue data area
*       U8 *queue_size              size of the queue [number of entries available when empty]
*       U8 *available               number of unused/available queue entries
*       U8 *messages                number of messages in the queue
*       U8 *tasks_waiting           number of waiting tasks [if used, otherwise 0]
*       NUSE_TASK *first_task       first waiting task [if used, otherwise 0]
*
*************************************************************************/

#if NUSE_QUEUE_INFORMATION || (NUSE_INCLUDE_EVERYTHING && (NUSE_QUEUE_NUMBER != 0))

    STATUS NUSE_Queue_Information(NUSE_QUEUE queue, ADDR *start_address, U8 *queue_size, U8 *available, U8 *messages, U8 *tasks_waiting, NUSE_TASK *first_task)
    {
        #if NUSE_API_PARAMETER_CHECKING
            if (queue >= NUSE_QUEUE_NUMBER)
            {
                return NUSE_INVALID_QUEUE;
            }

            if ((start_address == NULL) || (queue_size == NULL) || (available == NULL) || (messages == NULL) || (tasks_waiting == NULL) || (first_task == NULL))
            {
                return NUSE_INVALID_POINTER;
            }
        #endif

        *start_address = NUSE_Queue_Data[queue];
        *queue_size = NUSE_Queue_Size[queue];
        *available = NUSE_Queue_Size[queue] - NUSE_Queue_Items[queue];
        *messages = NUSE_Queue_Items[queue];

        #if NUSE_BLOCKING_ENABLE

            *tasks_waiting = NUSE_Queue_Blocking_Count[queue];
            if (NUSE_Queue_Blocking_Count[queue] != 0)
            {
                U8 index;

                for (index=0; index<NUSE_TASK_NUMBER; index++)
                {
                    if ((LONIB(NUSE_Task_Status[index]) == NUSE_QUEUE_SUSPEND)
                        && (HINIB(NUSE_Task_Status[index]) == queue))
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
*       NUSE_Queue_Count()
*
*   DESCRIPTION
*
*       Returns the number of queues in the system
*       Code is trivial, as the number of queues is fixed in Nucleus SE,
*       so a constant is returned
*
*   INPUTS
*
*       (none)
*
*   RETURNS
*
*       U8 value        number of queues in the system [Value: 0-15]
*
*   OTHER OUTPUTS
*
*       (none)
*
*************************************************************************/

#if NUSE_QUEUE_COUNT || NUSE_INCLUDE_EVERYTHING

    U8 NUSE_Queue_Count(void)
    {
        return NUSE_QUEUE_NUMBER;
    }

#endif
