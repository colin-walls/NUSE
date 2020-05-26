/*** Event Group handling APIs ***/

/************************************************************************
*
*   This file contains the definitions of functions concerned with the
*   handling of event groups in Nucleus SE:
*
*   NUSE_Event_Group_Set()
*   NUSE_Event_Group_Retrieve()
*   NUSE_Event_Group_Information()
*   NUSE_Event_Group_Count()
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
*       NUSE_Event_Group_Set()
*
*   DESCRIPTION
*
*       Sets [OR operation] or clears [AND operation] flags within a group
*
*   INPUTS
*
*       NUSE_EVENT_GROUP group      index of event flag group
*       U8 event_flags              bit pattern to specify setting/clearing
*       OPTION operation            required operation; may be: NUSE_OR or NUSE_AND
*
*   RETURNS
*
*       NUSE_SUCCESS                event flags set/cleared successfully
*       NUSE_INVALID_GROUP          specified event group index is invalid
*       NUSE_INVALID_OPERATION      the specified operation [operation] was not NUSE_OR or NUSE_AND
*
*   OTHER OUTPUTS
*
*       (none)
*
*************************************************************************/

#if NUSE_EVENT_GROUP_SET || (NUSE_INCLUDE_EVERYTHING && (NUSE_EVENT_GROUP_NUMBER != 0))

    STATUS NUSE_Event_Group_Set(NUSE_EVENT_GROUP group, U8 event_flags, OPTION operation)
    {
        STATUS return_value;

        #if NUSE_API_PARAMETER_CHECKING
            if (group >= NUSE_EVENT_GROUP_NUMBER)
            {
                return NUSE_INVALID_GROUP;
            }

            if ((operation != NUSE_OR) && (operation != NUSE_AND))
            {
                return_value = NUSE_INVALID_OPERATION;
            }
        #endif

        NUSE_CS_Enter();

        if (operation == NUSE_OR)
        {
            NUSE_Event_Group_Data[group] |= event_flags;
        }
        else  /* NUSE_AND */
        {
            NUSE_Event_Group_Data[group] &= event_flags;
        }

        #if NUSE_BLOCKING_ENABLE

            while (NUSE_Event_Group_Blocking_Count[group] != 0)
            {
                U8 index;                                       /* check whether any tasks are blocked */
                                                                /* on this event group */

                for (index=0; index<NUSE_TASK_NUMBER; index++)
                {
                    if ((LONIB(NUSE_Task_Status[index]) == NUSE_EVENT_SUSPEND)
                        && (HINIB(NUSE_Task_Status[index]) == group))
                    {
                        NUSE_Task_Blocking_Return[index] = NUSE_SUCCESS;
                        NUSE_Task_Status[index] = NUSE_READY;
                        break;
                    }
                }
                NUSE_Event_Group_Blocking_Count[group]--;
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
*       NUSE_Event_Group_Retrieve()
*
*   DESCRIPTION
*
*       Obtains the state of particular event flags within the specified group
*       A bit pattern [requested_events] determines which flags are considered
*       OR operations return success if any flags are set
*       AND operations return success only if all the flags are set
*
*   INPUTS
*
*       NUSE_EVENT_GROUP group      index of event flag group
*       U8 requested_events         bit pattern specifying required flags
*       OPTION operation            required operation; may be: NUSE_OR or NUSE_AND
*       U8 *retrieved_events        pointer to storage for retrieved events
*       U8 suspend                  task suspend option - must be NUSE_NO_SUSPEND or NUSE_SUSPEND
*                                   depending upon the setting of NUSE_BLOCKING_ENABLE
*
*   RETURNS
*
*       NUSE_SUCCESS                event flags retrieved successfully
*       NUSE_NOT_PRESENT            specified operation did not retrieve events
*                                   [none present for OR; not all present for AND]
*       NUSE_INVALID_GROUP          specified event group index is invalid
*       NUSE_INVALID_OPERATION      the specified operation [operation] was not NUSE_OR or NUSE_AND
*                                   [NUSE_OR_CONSUME and NUSE_AND_CONSUME not supported by Nucleus SE]
*       NUSE_INVALID_POINTER        pointer to storage for retieved events [retrieved_events] is NULL
*       NUSE_INVALID_SUSPEND        task suspend option is not set to NUSE_NO_SUSPEND or NUSE_SUSPEND
*
*   OTHER OUTPUTS
*
*       U8 *retrieved_events        successfully retrieved events
*
*************************************************************************/

#if NUSE_EVENT_GROUP_RETRIEVE || (NUSE_INCLUDE_EVERYTHING && (NUSE_EVENT_GROUP_NUMBER != 0))

    STATUS NUSE_Event_Group_Retrieve(NUSE_EVENT_GROUP group, U8 requested_events, OPTION operation, U8 *retrieved_events, U8 suspend)
    {
        STATUS return_value;
        U8 temp_events;

        #if NUSE_API_PARAMETER_CHECKING
            if (group >= NUSE_EVENT_GROUP_NUMBER)
            {
                return NUSE_INVALID_GROUP;
            }

            if (retrieved_events == NULL)
            {
                return NUSE_INVALID_POINTER;
            }

            if ((operation != NUSE_OR) && (operation != NUSE_AND))
            {
                return NUSE_INVALID_OPERATION;
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
                temp_events = NUSE_Event_Group_Data[group] & requested_events;

                if (operation == NUSE_OR)
                {
                    if (temp_events != 0)
                    {
                        return_value = NUSE_SUCCESS;
                    }
                    else
                    {
                        return_value = NUSE_NOT_PRESENT;
                    }
                }
                else    /* operation == NUSE_AND */
                {
                    if (temp_events == requested_events)
                    {
                        return_value = NUSE_SUCCESS;
                    }
                    else
                    {
                        return_value = NUSE_NOT_PRESENT;
                    }
                }

                if (return_value == NUSE_SUCCESS)
                {
                    suspend = NUSE_NO_SUSPEND;
                }
                else
                {
                    if (suspend == NUSE_SUSPEND)          /* block task */
                    {
                        NUSE_Event_Group_Blocking_Count[group]++;
                        NUSE_Suspend_Task(NUSE_Task_Active, (group << 4) | NUSE_EVENT_SUSPEND);
                        return_value = NUSE_Task_Blocking_Return[NUSE_Task_Active];
                        if (return_value != NUSE_SUCCESS)
                        {
                            suspend = NUSE_NO_SUSPEND;
                        }
                    }
                }
            } while (suspend == NUSE_SUSPEND);
        #else
            temp_events = NUSE_Event_Group_Data[group] & requested_events;

            if (operation == NUSE_OR)
            {
                if (temp_events != 0)
                {
                    return_value = NUSE_SUCCESS;
                }
                else
                {
                    return_value = NUSE_NOT_PRESENT;
                }
            }
            else    /* operation == NUSE_AND */
            {
                if (temp_events == requested_events)
                {
                    return_value = NUSE_SUCCESS;
                }
                else
                {
                    return_value = NUSE_NOT_PRESENT;
                }
            }
        #endif

        if (return_value == NUSE_SUCCESS)
        {
            *retrieved_events = temp_events;
        }

        NUSE_CS_Exit();

        return return_value;
    }

#endif


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Event_Group_Information()
*
*   DESCRIPTION
*
*       Returns a number of items of data about an event group
*       Unsupported items are handled in nuse.h
*
*   INPUTS
*
*       NUSE_EVENT_GROUP group      index of event group about which information is required
*       U8 *event_flags             pointer to storage for the current event flags setting
*       U8 *tasks_waiting           pointer to storage for number of waiting tasks
*       NUSE_TASK *first_task       pointer to storage for pointer to first waiting task
*
*   RETURNS
*
*       NUSE_SUCCESS                the event group status was successfully returned
*       NUSE_INVALID_GROUP          the event group index was not valid
*       NUSE_INVALID_POINTER        one or more of the pointer parameters was NULL
*
*   OTHER OUTPUTS
*
*       U8 *event_flags             current event flags setting
*       U8 *tasks_waiting           number of waiting tasks [if used - otherwise 0]
*       NUSE_TASK *first_task       first waiting task [if used - otherwise 0]
*
*************************************************************************/

#if NUSE_EVENT_GROUP_INFORMATION || (NUSE_INCLUDE_EVERYTHING && (NUSE_EVENT_GROUP_NUMBER != 0))

    STATUS NUSE_Event_Group_Information(NUSE_EVENT_GROUP group, U8 *event_flags, U8 *tasks_waiting, NUSE_TASK *first_task)
    {
        #if NUSE_API_PARAMETER_CHECKING
            if (group >= NUSE_EVENT_GROUP_NUMBER)
            {
                return NUSE_INVALID_GROUP;
            }
        #endif

        *event_flags = NUSE_Event_Group_Data[group];

        #if NUSE_BLOCKING_ENABLE

            *tasks_waiting = NUSE_Event_Group_Blocking_Count[group];
            if (NUSE_Event_Group_Blocking_Count[group] != 0)
            {
                U8 index;

                for (index=0; index<NUSE_TASK_NUMBER; index++)
                {
                    if ((LONIB(NUSE_Task_Status[index]) == NUSE_EVENT_SUSPEND)
                        && (HINIB(NUSE_Task_Status[index]) == group))
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
*       NUSE_Event_Group_Count()
*
*   DESCRIPTION
*
*       Returns the number of event groups in the system
*       Code is trivial, as the number of event groups is fixed in Nucleus SE,
*       so a constant is returned
*
*   INPUTS
*
*       (none)
*
*   RETURNS
*
*       U8 value        number of event groups in the system [Value: 0-15]
*
*   OTHER OUTPUTS
*
*       (none)
*
*************************************************************************/

#if NUSE_EVENT_GROUP_COUNT || NUSE_INCLUDE_EVERYTHING

    U8 NUSE_Event_Group_Count(void)
    {
        return NUSE_EVENT_GROUP_NUMBER;
    }

#endif
