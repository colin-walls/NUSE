/*** Pipe handling APIs ***/

/************************************************************************
*
*   This file contains the definitions of functions concerned with the
*   handling of pipes in Nucleus SE:
*
*   NUSE_Pipe_Send()
*   NUSE_Pipe_Receive()
*   NUSE_Pipe_Jam()
*   NUSE_Pipe_Reset()
*   NUSE_Pipe_Information()
*   NUSE_Pipe_Count()
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
*       NUSE_Pipe_Send()
*
*   DESCRIPTION
*
*       Sends a single (type U8[]) message to the pipe
*
*   INPUTS
*
*       NUSE_PIPE pipe          index of pipe to be used
*       U8 *message             pointer to the message to be sent
*       U8 size                 message size; must correspond to the size configured for the pipe [in nuse_config.c]
*       U8 suspend              task suspend option - must be NUSE_NO_SUSPEND or NUSE_SUSPEND
*                               depending upon the setting of NUSE_BLOCKING_ENABLE
*
*   RETURNS
*
*       NUSE_SUCCESS            message successfully sent/received
*       NUSE_PIPE_FULL          send failed as there was no room in the pipe
*       NUSE_INVALID_PIPE       specified pipe index is invalid
*       NUSE_INVALID_POINTER    message pointer [message] is NULL
*       NUSE_INVALID_SIZE       message size parameter [size] did not correspond to the size configured for the pipe
*       NUSE_INVALID_SUSPEND    task suspend option is not set to NUSE_NO_SUSPEND or NUSE_SUSPEND
*
*   OTHER OUTPUTS
*
*       (none)
*
*************************************************************************/

#if NUSE_PIPE_SEND || (NUSE_INCLUDE_EVERYTHING && (NUSE_PIPE_NUMBER != 0))

    STATUS NUSE_Pipe_Send(NUSE_PIPE pipe, U8 *message, U8 size, U8 suspend)
    {
        U8 i, msgsize, *data;
        STATUS return_value;

        #if NUSE_API_PARAMETER_CHECKING
            if (pipe >= NUSE_PIPE_NUMBER)
            {
                return NUSE_INVALID_PIPE;
            }

            if (message == NULL)
            {
                return NUSE_INVALID_POINTER;
            }

            if (size != NUSE_Pipe_Message_Size[pipe])
            {
                return NUSE_INVALID_SIZE;
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

        msgsize = NUSE_Pipe_Message_Size[pipe];

        NUSE_CS_Enter();

        #if NUSE_BLOCKING_ENABLE
            do
            {
                if (NUSE_Pipe_Items[pipe] == NUSE_Pipe_Size[pipe])          /* pipe full */
                {
                    if (suspend == NUSE_NO_SUSPEND)
                    {
                        return_value = NUSE_PIPE_FULL;
                    }
                    else
                    {                                                       /* block task */
                        NUSE_Pipe_Blocking_Count[pipe]++;
                        NUSE_Suspend_Task(NUSE_Task_Active, (pipe << 4) | NUSE_PIPE_SUSPEND);
                        return_value = NUSE_Task_Blocking_Return[NUSE_Task_Active];
                        if (return_value != NUSE_SUCCESS)
                        {
                            suspend = NUSE_NO_SUSPEND;
                        }
                    }
                }
                else
                {                                                           /* pipe element available */
                    data = &NUSE_Pipe_Data[pipe][NUSE_Pipe_Head[pipe]];
                    for (i=0; i<msgsize; i++)
                    {
                        *data++ = *message++;
                    }

                    NUSE_Pipe_Head[pipe] += msgsize;
                    if (NUSE_Pipe_Head[pipe] == (NUSE_Pipe_Size[pipe] * msgsize))
                    {
                        NUSE_Pipe_Head[pipe] = 0;
                    }

                    NUSE_Pipe_Items[pipe]++;
                    if (NUSE_Pipe_Blocking_Count[pipe] != 0)
                    {
                        U8 index;                                           /* check whether a task is blocked */
                                                                            /* on this pipe */

                        NUSE_Pipe_Blocking_Count[pipe]--;
                        for (index=0; index<NUSE_TASK_NUMBER; index++)
                        {
                            if ((LONIB(NUSE_Task_Status[index]) == NUSE_PIPE_SUSPEND)
                                && (HINIB(NUSE_Task_Status[index]) == pipe))
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
            if (NUSE_Pipe_Items[pipe] == NUSE_Pipe_Size[pipe])              /* pipe full */
            {
                return_value = NUSE_PIPE_FULL;
            }
            else                                                            /* pipe element available */
            {
                data = &NUSE_Pipe_Data[pipe][NUSE_Pipe_Head[pipe]];
                for (i=0; i<msgsize; i++)
                {
                    *data++ = *message++;
                }

                NUSE_Pipe_Head[pipe] += msgsize;
                if (NUSE_Pipe_Head[pipe] == (NUSE_Pipe_Size[pipe] * msgsize))
                {
                    NUSE_Pipe_Head[pipe] = 0;
                }

                NUSE_Pipe_Items[pipe]++;
                return_value = NUSE_SUCCESS;
            }
        #endif

        NUSE_CS_Exit();

        return return_value ;
    }

#endif


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Pipe_Receive()
*
*   DESCRIPTION
*
*       Receives a single (type U8[]) message from the pipe
*
*   INPUTS
*
*       NUSE_PIPE pipe          index of pipe to be used
*       U8 *message             pointer to storage for message to be received
*       U8 size                 message size; must correspond to the size configured for the pipe [in nuse_config.c]
*       U8 *actual_size         pointer to storage for the message size
*       U8 suspend              task suspend option - must be NUSE_NO_SUSPEND or NUSE_SUSPEND
*                               depending upon the setting of NUSE_BLOCKING_ENABLE
*
*   RETURNS
*
*       NUSE_SUCCESS            message successfully sent/received
*       NUSE_PIPE_EMPTY         receive failed as no message was waiting
*       NUSE_INVALID_PIPE       specified pipe index is invalid
*       NUSE_INVALID_POINTER    message pointer [message] is NULL
*       NUSE_INVALID_SIZE       message size parameter [size] correspond to the size configured for the pipe
*       NUSE_INVALID_SUSPEND    task suspend option is not set to NUSE_NO_SUSPEND or NUSE_SUSPEND
*
*   OTHER OUTPUTS
*
*       U8 *message             received message
*       U8 *actual_size         message size
*
*************************************************************************/

#if NUSE_PIPE_RECEIVE || (NUSE_INCLUDE_EVERYTHING && (NUSE_PIPE_NUMBER != 0))

    STATUS NUSE_Pipe_Receive(NUSE_PIPE pipe, U8 *message, U8 size, U8 *actual_size, U8 suspend)
    {
        U8 i, msgsize, *data;
        STATUS return_value;

        #if NUSE_API_PARAMETER_CHECKING
            if (pipe >= NUSE_PIPE_NUMBER)
            {
                return NUSE_INVALID_PIPE;
            }

            if (size != NUSE_Pipe_Message_Size[pipe])
            {
                return NUSE_INVALID_SIZE;
            }

            if (message == NULL || actual_size == NULL)
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

        msgsize = NUSE_Pipe_Message_Size[pipe];

        NUSE_CS_Enter();

        #if NUSE_BLOCKING_ENABLE
            do
            {
                if (NUSE_Pipe_Items[pipe] == 0)                             /* pipe empty */
                {
                    if (suspend == NUSE_NO_SUSPEND)
                    {
                        return_value = NUSE_PIPE_EMPTY;
                    }
                    else
                    {                                                       /* block task */
                        NUSE_Pipe_Blocking_Count[pipe]++;
                        NUSE_Suspend_Task(NUSE_Task_Active, (pipe << 4) | NUSE_PIPE_SUSPEND);
                        return_value = NUSE_Task_Blocking_Return[NUSE_Task_Active];
                        if (return_value != NUSE_SUCCESS)
                        {
                            suspend = NUSE_NO_SUSPEND;
                        }
                    }
                }
                else
                {                                                           /* message available */
                    data = &NUSE_Pipe_Data[pipe][NUSE_Pipe_Tail[pipe]];
                    for (i=0; i<msgsize; i++)
                    {
                         *message++ = *data++;
                    }

                    NUSE_Pipe_Tail[pipe] += msgsize;
                    if (NUSE_Pipe_Tail[pipe] == (NUSE_Pipe_Size[pipe] * msgsize))
                    {
                        NUSE_Pipe_Tail[pipe] = 0;
                    }

                    NUSE_Pipe_Items[pipe]--;
                    if (NUSE_Pipe_Blocking_Count[pipe] != 0)
                    {
                        U8 index;                                           /* check whether a task is blocked */
                                                                            /* on this pipe */

                        NUSE_Pipe_Blocking_Count[pipe]--;
                        for (index=0; index<NUSE_TASK_NUMBER; index++)
                        {
                            if ((LONIB(NUSE_Task_Status[index]) == NUSE_PIPE_SUSPEND)
                                && (HINIB(NUSE_Task_Status[index]) == pipe))
                            {
                                NUSE_Task_Blocking_Return[index] = NUSE_SUCCESS;
                                NUSE_Wake_Task(index);
                                break;
                            }
                        }
                    }

                    *actual_size = msgsize;
                    return_value = NUSE_SUCCESS;
                    suspend = NUSE_NO_SUSPEND;
                }
            } while (suspend == NUSE_SUSPEND);
        #else
            if (NUSE_Pipe_Items[pipe] == 0)                                 /* pipe empty */
            {
                return_value = NUSE_PIPE_EMPTY;
            }
            else
            {                                                               /* message available */
                data = &NUSE_Pipe_Data[pipe][NUSE_Pipe_Tail[pipe]];
                for (i=0; i<msgsize; i++)
                {
                     *message++ = *data++;
                }

                NUSE_Pipe_Tail[pipe] += msgsize;
                if (NUSE_Pipe_Tail[pipe] == (NUSE_Pipe_Size[pipe] * msgsize))
                {
                    NUSE_Pipe_Tail[pipe] = 0;
                }

                NUSE_Pipe_Items[pipe]--;

                *actual_size = msgsize;
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
*       NUSE_Pipe_Jam()
*
*   DESCRIPTION
*
*       Sends a single (type U8[]) message to the front of the pipe [also known as "jamming" a message]
*
*   INPUTS
*
*       NUSE_PIPE pipe          index of pipe to be used
*       U8 *message             pointer to the message to be sent
*       U8 size                 message size; must correspond to the size configured for the pipe [in nuse_config.c]
*       U8 suspend              task suspend option - must be NUSE_NO_SUSPEND or NUSE_SUSPEND
*                               depending upon the setting of NUSE_BLOCKING_ENABLE
*
*   RETURNS
*
*       NUSE_SUCCESS            message successfully sent/received
*       NUSE_PIPE_FULL          send failed as there was no room in the pipe
*       NUSE_INVALID_PIPE       specified pipe index is invalid
*       NUSE_INVALID_POINTER    message pointer [message] is NULL
*       NUSE_INVALID_SIZE       message size parameter [size] did not correspond to the size configured for the pipe
*       NUSE_INVALID_SUSPEND    task suspend option is not set to NUSE_NO_SUSPEND or NUSE_SUSPEND
*
*   OTHER OUTPUTS
*
*       (none)
*
*************************************************************************/

#if NUSE_PIPE_JAM || (NUSE_INCLUDE_EVERYTHING && (NUSE_PIPE_NUMBER != 0))

    STATUS NUSE_Pipe_Jam(NUSE_PIPE pipe, U8 *message, U8 size, U8 suspend)
    {
        U8 i, msgsize, *data;
        STATUS return_value;

        #if NUSE_API_PARAMETER_CHECKING
            if (pipe >= NUSE_PIPE_NUMBER)
            {
                return NUSE_INVALID_PIPE;
            }

            if (message == NULL)
            {
                return NUSE_INVALID_POINTER;
            }

            if (size != NUSE_Pipe_Message_Size[pipe])
            {
                return NUSE_INVALID_SIZE;
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

        msgsize = NUSE_Pipe_Message_Size[pipe];

        NUSE_CS_Enter();

        #if NUSE_BLOCKING_ENABLE
            do
            {
                if (NUSE_Pipe_Items[pipe] == NUSE_Pipe_Size[pipe])          /* pipe full */
                {
                    if (suspend == NUSE_NO_SUSPEND)
                    {
                        return_value = NUSE_PIPE_FULL;
                    }
                    else
                    {                                                       /* block task */
                        NUSE_Pipe_Blocking_Count[pipe]++;
                        NUSE_Suspend_Task(NUSE_Task_Active, (pipe << 4) | NUSE_PIPE_SUSPEND);
                        return_value = NUSE_Task_Blocking_Return[NUSE_Task_Active];
                        if (return_value != NUSE_SUCCESS)
                        {
                            suspend = NUSE_NO_SUSPEND;
                        }
                    }
                }
                else
                {                                                           /* pipe element available */
                    if (NUSE_Pipe_Tail[pipe] == 0)
                    {
                        NUSE_Pipe_Tail[pipe] = (NUSE_Pipe_Size[pipe] - 1) * msgsize;
                    }
                    else
                    {
                        NUSE_Pipe_Tail[pipe] -= msgsize;
                    }
                    data = &NUSE_Pipe_Data[pipe][NUSE_Pipe_Tail[pipe]];
                    for (i=0; i<msgsize; i++)
                    {
                        *data++ = *message++;
                    }
                    NUSE_Pipe_Items[pipe]++;
                    if (NUSE_Pipe_Blocking_Count[pipe] != 0)
                    {
                        U8 index;                                           /* check whether a task is blocked */
                                                                            /* on this pipe */

                        NUSE_Pipe_Blocking_Count[pipe]--;
                        for (index=0; index<NUSE_TASK_NUMBER; index++)
                        {
                            if ((LONIB(NUSE_Task_Status[index]) == NUSE_PIPE_SUSPEND)
                                && (HINIB(NUSE_Task_Status[index]) == pipe))
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
            if (NUSE_Pipe_Items[pipe] == NUSE_Pipe_Size[pipe])              /* pipe full */
            {
                return_value = NUSE_PIPE_FULL;
            }
            else                                                            /* pipe element available */
            {
                if (NUSE_Pipe_Tail[pipe] == 0)
                {
                    NUSE_Pipe_Tail[pipe] = (NUSE_Pipe_Size[pipe] - 1) * msgsize;
                }
                else
                {
                    NUSE_Pipe_Tail[pipe] -= msgsize;
                }
                data = &NUSE_Pipe_Data[pipe][NUSE_Pipe_Tail[pipe]];
                for (i=0; i<msgsize; i++)
                {
                    *data++ = *message++;
                }
                NUSE_Pipe_Items[pipe]++;
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
*       NUSE_Pipe_Reset()
*
*   DESCRIPTION
*
*       Restores the specified pipe to its initialized state
*       Any waiting messages are lost
*       Uses the same initialization routine as on start-up - in nuse_init.c
*
*   INPUTS
*
*       NUSE_PIPE pipe      index of pipe to be reset
*
*   RETURNS
*
*       NUSE_SUCCESS        pipe was successfully reset
*       NUSE_INVALID_PIPE   pipe index was not valid
*
*   OTHER OUTPUTS
*
*       (none)
*
*************************************************************************/

#if NUSE_PIPE_RESET || (NUSE_INCLUDE_EVERYTHING && (NUSE_PIPE_NUMBER != 0))

    STATUS NUSE_Pipe_Reset(NUSE_PIPE pipe)
    {
        #if NUSE_API_PARAMETER_CHECKING
            if (pipe >= NUSE_PIPE_NUMBER)
            {
                return NUSE_INVALID_PIPE;
            }
        #endif

        NUSE_CS_Enter();

        NUSE_Pipe_Head[pipe] = 0;
        NUSE_Pipe_Tail[pipe] = 0;
        NUSE_Pipe_Items[pipe] = 0;

        #if NUSE_BLOCKING_ENABLE

            while (NUSE_Pipe_Blocking_Count[pipe] != 0)
            {
                U8 index;                                           /* check whether any tasks are blocked */
                                                                    /* on this pipe */

                for (index=0; index<NUSE_TASK_NUMBER; index++)
                {
                    if ((LONIB(NUSE_Task_Status[index]) == NUSE_PIPE_SUSPEND)
                        && (HINIB(NUSE_Task_Status[index]) == pipe))
                    {
                        NUSE_Task_Blocking_Return[index] = NUSE_PIPE_RESET;
                        NUSE_Task_Status[index] = NUSE_READY;
                        break;
                    }
                }
                NUSE_Pipe_Blocking_Count[pipe]--;
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
*       NUSE_Pipe_Information()
*
*   DESCRIPTION
*
*       Returns a number of items of data about a pipe
*       Unsupported items are handled in nuse.h
*
*   INPUTS
*
*       NUSE_PIPE pipe              index of pipe about which information is required
*       ADDR *start_address         pointer to storage for the start address of the pipe
*       U8 *pipe_size               pointer to storage for the size of the pipe
*       U8 *available               pointer to storage for the number of free pipe entries
*       U8 *messages                pointer to storage for the number of messages in the pipe
*       U8 *message_size            pointer to storage for the size of messages handled by the pipe
*       U8 *tasks_waiting           pointer to storage for number of waiting tasks [if used]
*       NUSE_TASK *first_task       pointer to storage for pointer to first waiting task [if used]
*
*   RETURNS
*
*       NUSE_SUCCESS                the pipe status was successfully returned
*       NUSE_INVALID_PIPE           the pipe index was not valid
*       NUSE_INVALID_POINTER        one or more of the pointer parameters was NULL
*
*   OTHER OUTPUTS
*
*       ADDR *start_address         start address of the pipe data area
*       U8 *pipe_size               size of the pipe [number of entries available when empty]
*       U8 *available               number of unused/available pipe entries
*       U8 *messages                number of messages in the pipe
*       U8 *message_size            the size of messages handled by the pipe [set in nuse_config.c]
*       U8 *tasks_waiting           number of waiting tasks [if used, otherwise 0]
*       NUSE_TASK *first_task       first waiting task [if used, otherwise 0]
*
*************************************************************************/

#if NUSE_PIPE_INFORMATION || (NUSE_INCLUDE_EVERYTHING && (NUSE_PIPE_NUMBER != 0))

    STATUS NUSE_Pipe_Information(NUSE_PIPE pipe, ADDR *start_address, U8 *pipe_size, U8 *available, U8 *messages, U8 *message_size, U8 *tasks_waiting, NUSE_TASK *first_task)
    {
        #if NUSE_API_PARAMETER_CHECKING
            if (pipe >= NUSE_PIPE_NUMBER)
            {
                return NUSE_INVALID_PIPE;
            }

            if ((start_address == NULL) || (pipe_size == NULL) || (messages == NULL) || (available == NULL) || (message_size == NULL) || (tasks_waiting == NULL) || (first_task == NULL))
            {
                return NUSE_INVALID_POINTER;
            }
        #endif

        *start_address = NUSE_Pipe_Data[pipe];
        *pipe_size = NUSE_Pipe_Size[pipe];
        *available = NUSE_Pipe_Size[pipe] - NUSE_Pipe_Items[pipe];
        *messages = NUSE_Pipe_Items[pipe];
        *message_size = NUSE_Pipe_Message_Size[pipe];

        #if NUSE_BLOCKING_ENABLE

            *tasks_waiting = NUSE_Pipe_Blocking_Count[pipe];
            if (NUSE_Pipe_Blocking_Count[pipe] != 0)
            {
                U8 index;

                for (index=0; index<NUSE_TASK_NUMBER; index++)
                {
                    if ((LONIB(NUSE_Task_Status[index]) == NUSE_PIPE_SUSPEND)
                        && (HINIB(NUSE_Task_Status[index]) == pipe))
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
*       NUSE_Pipe_Count()
*
*   DESCRIPTION
*
*       Returns the number of pipes in the system
*       Code is trivial, as the number of pipes is fixed in Nucleus SE,
*       so a constant is returned
*
*   INPUTS
*
*       (none)
*
*   RETURNS
*
*       U8 value        number of pipes in the system [Value: 0-15]
*
*   OTHER OUTPUTS
*
*       (none)
*
*************************************************************************/

#if NUSE_PIPE_COUNT || NUSE_INCLUDE_EVERYTHING

    U8 NUSE_Pipe_Count(void)
    {
        return NUSE_PIPE_NUMBER;
    }

#endif
